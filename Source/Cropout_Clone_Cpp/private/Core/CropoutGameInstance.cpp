// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"

#include "Core/InGame/InGameGameMode.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Global/GlobalSharedVariable.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Base/SystemManager.h"

UCropoutGameInstance* UCropoutGameInstance::Instance = nullptr;

void UCropoutGameInstance::Init()
{
	Super::Init();
	Instance = this;

	GlobalSharedVariable = NewObject<UGlobalSharedVariable>();
	GlobalSharedVariable->SetCropoutGameInstance(this);

	InitializeSystemManagers();
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("UCropoutGameInstance::Shutdown"));

	if((GIsEditor && IsEngineExitRequested()) == false)
	{
		SaveOptionData();

		if(SystemManagerMap.IsEmpty() == false)
		{
			for(auto& Pair : SystemManagerMap)
			{
				if(IsValid(Pair.Value))
				{
					Pair.Value->Release();
				}
			}

			for(auto& Pair : SystemManagerMap)
			{
				if(IsValid(Pair.Value))
				{
					Pair.Value->Destroy();

					if(Managers.Contains(Pair.Value))
					{
						Managers.Remove(Pair.Value);
					}
				}
			}
			SystemManagerMap.Empty();
		}
	}

	Instance = nullptr;

	Super::Shutdown();
}

void UCropoutGameInstance::SetCurrentLevelScript(ACropoutLevelScriptBase* cropoutLevelScriptBase)
{
	CurrentLevelScript = cropoutLevelScriptBase;

	if(CurrentLevelScript != nullptr)
	{
		for(auto& Pair : SystemManagerMap)
		{
			if(IsValid(Pair.Value))
			{
				Pair.Value->OnLevelLoaded();
			}
		}

		if(IsLevelManagerInitialized == false)
		{
			InitializeLevelManagers();
		}
	}
	else
	{
		ReleaseLevelManagers();

		if(SystemManagerMap.IsEmpty() == false)
		{
			for(auto& Pair : SystemManagerMap)
			{
				if(IsValid(Pair.Value))
				{
					Pair.Value->OnLevelDestroy();
				}
			}
		}
	}
}

void UCropoutGameInstance::InitializeSystemManagers()
{
	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(USystemManager::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			USystemManager* SystemManager = NewObject<USystemManager>(this, Class);
			SystemManager->SetGameInstance(this);
			SystemManager->AddOnBeginDestroyHandle(this, &UCropoutGameInstance::SystemManagerBeginDestroy);
			SystemManagerMap.Add(Class, SystemManager);
			Managers.Add(SystemManager);
		}
	}

	for(auto& Pair : SystemManagerMap)
	{
		if(IsValid(Pair.Value))
		{
			Pair.Value->Init();
		}
	}

	for(auto& Pair : SystemManagerMap)
	{
		if(IsValid(Pair.Value))
		{
			Pair.Value->Prepare();
		}
	}

	LoadOptionData();
}

void UCropoutGameInstance::InitializeLevelManagers()
{
	IsProgressLevelManagerInitialized = true;
	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(ULevelManager::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			ULevelManager* LevelManager = NewObject<ULevelManager>(this, Class);
			LevelManager->SetGameInstance(this);
			LevelManager->AddOnBeginDestroyHandle(this, &UCropoutGameInstance::LevelManagerBeginDestroy);
			LevelManagerMap.Add(Class, LevelManager);
			Managers.Add(LevelManager);
		}
	}

	for(auto& Pair : LevelManagerMap)
	{
		if(IsValid(Pair.Value))
		{
			Pair.Value->Init();
		}
	}

	IsLevelManagerInitialized = true;
	IsProgressLevelManagerInitialized = false;

	for(auto& Pair : LevelManagerMap)
	{
		if(IsValid(Pair.Value))
		{
			Pair.Value->Prepare();
		}
	}
}

void UCropoutGameInstance::ReleaseLevelManagers()
{
	if(LevelManagerMap.IsEmpty() == false)
	{
		for(auto& Pair : LevelManagerMap)
		{
			if(IsValid(Pair.Value))
			{
				Pair.Value->ClearOnBeginDestroyHandle();
			}
		}

		for(auto& Pair : LevelManagerMap)
		{
			if(IsValid(Pair.Value))
			{
				Pair.Value->Release();
			}
		}

		for(auto& Pair : LevelManagerMap)
		{
			if(IsValid(Pair.Value))
			{
				Pair.Value->Destroy();

				if(Managers.Contains(Pair.Value))
				{
					Managers.Remove(Pair.Value);
				}
			}
		}

		LevelManagerMap.Empty();
	}

	IsLevelManagerInitialized = false;
	IsProgressLevelManagerInitialized = false;
}

void UCropoutGameInstance::SystemManagerBeginDestroy(UManagerBase* Manager)
{
	SystemManagerMap.Remove(Manager->GetClass());

	if(Managers.Contains(Manager))
	{
		Managers.Remove(Manager);
	}
}

void UCropoutGameInstance::LevelManagerBeginDestroy(UManagerBase* Manager)
{
	LevelManagerMap.Remove(Manager->GetClass());

	if(Managers.Contains(Manager))
	{
		Managers.Remove(Manager);
	}
}

void UCropoutGameInstance::SaveOptionData()
{
	UOptionSaveData* OptionSaveData = Cast<UOptionSaveData>(
		UGameplayStatics::CreateSaveGameObject(UOptionSaveData::StaticClass()));

	for(auto systemManagerTuple : SystemManagerMap)
	{
		systemManagerTuple.Value->OnSaveData(OptionSaveData);
	}

	UGameplayStatics::SaveGameToSlot(OptionSaveData, TEXT("OptionSaveSlot"), 0);
}

void UCropoutGameInstance::LoadOptionData()
{
	UOptionSaveData* OptionSaveData = Cast<UOptionSaveData>(
		UGameplayStatics::LoadGameFromSlot(TEXT("OptionSaveSlot"), 0));

	if(OptionSaveData == nullptr)
	{
		return;
	}

	for(auto systemManagerTuple : SystemManagerMap)
	{
		systemManagerTuple.Value->OnLoadData(OptionSaveData);
	}
}

void UCropoutGameInstance::SaveInGameData()
{
	UInGameSaveData* InGameSaveData = Cast<UInGameSaveData>(
		UGameplayStatics::CreateSaveGameObject(UInGameSaveData::StaticClass()));

	for(auto levelManagerTuple : LevelManagerMap)
	{
		levelManagerTuple.Value->OnSaveData(InGameSaveData);
	}

	UGameplayStatics::SaveGameToSlot(InGameSaveData, TEXT("InGameSaveSlot"), 0);
}

void UCropoutGameInstance::LoadInGameData()
{
	UInGameSaveData* InGameSaveData = Cast<UInGameSaveData>(
		UGameplayStatics::LoadGameFromSlot(TEXT("InGameSaveSlot"), 0));

	if(InGameSaveData == nullptr)
	{
		return;
	}

	for(auto levelManagerTuple : LevelManagerMap)
	{
		levelManagerTuple.Value->OnLoadData(InGameSaveData);
	}
}

void UCropoutGameInstance::SetCurrentGameMode(AIslandBaseGameMode* gameMode)
{
	CurrentIslandGameMode = gameMode;
	CurrentInGameMode = nullptr;
	if(gameMode && gameMode->IsA(AInGameGameMode::StaticClass()))
	{
		CurrentInGameMode = Cast<AInGameGameMode>(gameMode);
	}
}

void UCropoutGameInstance::SetCurrentGameMode(AInGameGameMode* gameMode)
{
	CurrentIslandGameMode = gameMode;
	CurrentInGameMode = gameMode;
}

void UCropoutGameInstance::SetCurrentPlayerController(APlayerController* playerController)
{
	CurrentPlayerController = playerController;

	CurrentCropoutPlayerController = nullptr;
	if(playerController && playerController->IsA(AInGamePlayerController::StaticClass()))
	{
		CurrentCropoutPlayerController = Cast<AInGamePlayerController>(playerController);
	}
}

APlayerController* UCropoutGameInstance::GetCurrentPlayerController()
{
	if(CurrentPlayerController == nullptr)
	{
		SetCurrentPlayerController(GetFirstLocalPlayerController());
	}

	return CurrentPlayerController;
}

void UCropoutGameInstance::SetCurrentCropoutPlayerController(AInGamePlayerController* playerController)
{
	CurrentPlayerController = playerController;
	CurrentCropoutPlayerController = playerController;
}

AInGamePlayerController* UCropoutGameInstance::GetCurrentCropoutPlayerController()
{
	if(CurrentCropoutPlayerController == nullptr)
	{
		SetCurrentPlayerController(GetFirstLocalPlayerController());
	}

	return CurrentCropoutPlayerController;
}
