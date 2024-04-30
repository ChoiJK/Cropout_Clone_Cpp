// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"

#include "Core/CropoutPlayerController.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Data/SaveData.h"
#include "Manager/Base/SystemManager.h"

UCropoutGameInstance* UCropoutGameInstance::Instance = nullptr;

void UCropoutGameInstance::Init()
{
	Super::Init();
	Instance = this;

	GlobalEventDispatcher = MakeShared<FGlobalEventDispatcher>();

	GlobalSharedVariable = NewObject<UGlobalSharedVariable>();
	GlobalSharedVariable->SetCropoutGameInstance(this);

	SaveData = NewObject<USaveData>();

	InitializeSystemManagers();
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("UCropoutGameInstance::Shutdown"));

	if((GIsEditor && IsEngineExitRequested()) == false)
	{
		for(auto& Pair : SystemManagerMap)
		{
			if(Pair.Value != nullptr)
			{
				Pair.Value->Release();
			}
		}

		for(auto& Pair : SystemManagerMap)
		{
			if(IsValid(Pair.Value))
			{
				Pair.Value->Destroy();
			}
		}
		SystemManagerMap.Empty();
	}


	Super::Shutdown();
}

void UCropoutGameInstance::SetCurrentLevelScript(ACropoutLevelScriptBase* cropoutLevelScriptBase)
{
	CurrentLevelScript = cropoutLevelScriptBase;

	if(CurrentLevelScript != nullptr)
	{
		for(auto& Pair : SystemManagerMap)
		{
			Pair.Value->OnLevelLoaded();
		}

		if(IsLevelManagerInitialized == false)
		{
			InitializeLevelManagers();
		}
	}
	else
	{
		ReleaseLevelManagers();

		for(auto& Pair : SystemManagerMap)
		{
			Pair.Value->OnLevelDestroy();
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
		}
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Init();
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Prepare();
	}
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
		}
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Init();
	}

	IsLevelManagerInitialized = true;
	IsProgressLevelManagerInitialized = false;

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Prepare();
	}
}

void UCropoutGameInstance::ReleaseLevelManagers()
{
	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->ClearOnBeginDestroyHandle();
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Release();
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Destroy();
	}

	LevelManagerMap.Empty();

	IsLevelManagerInitialized = false;
	IsProgressLevelManagerInitialized = false;
}

void UCropoutGameInstance::SystemManagerBeginDestroy(UManagerBase* Manager)
{
	SystemManagerMap.Remove(Manager->GetClass());
}

void UCropoutGameInstance::LevelManagerBeginDestroy(UManagerBase* Manager)
{
	LevelManagerMap.Remove(Manager->GetClass());
}

void UCropoutGameInstance::SetCurrentPlayerController(APlayerController* playerController)
{
	CurrentPlayerController = playerController;

	if(playerController->IsA(ACropoutPlayerController::StaticClass()))
	{
		CurrentCropoutPlayerController = Cast<ACropoutPlayerController>(playerController);
	}
	else
	{
		CurrentCropoutPlayerController = nullptr;
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

void UCropoutGameInstance::SetCurrentCropoutPlayerController(ACropoutPlayerController* playerController)
{
	CurrentPlayerController = playerController;
	CurrentCropoutPlayerController = playerController;
}

ACropoutPlayerController* UCropoutGameInstance::GetCurrentCropoutPlayerController()
{
	if(CurrentCropoutPlayerController == nullptr)
	{
		SetCurrentPlayerController(GetFirstLocalPlayerController());
	}

	return CurrentCropoutPlayerController;
}
