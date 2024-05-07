// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Global/GlobalSharedVariable.h"
#include "Engine/StreamableManager.h"
#include "Data/SaveData.h"
#include "Level/CropoutLevelScriptBase.h"

#include "CropoutGameInstance.generated.h"

class AInGamePlayerController;
class AInGameGameMode;
class AIslandBaseGameMode;
class ACropoutLevelScriptBase;
class USystemManager;
class ULevelManager;

UCLASS()
class CROPOUT_CLONE_CPP_API UCropoutGameInstance : public UGameInstance
{
	GENERATED_BODY()


	UPROPERTY()
	USaveData* SaveData;

	UPROPERTY()
	UGlobalSharedVariable* GlobalSharedVariable;
	FStreamableManager streamableManager;

	UPROPERTY()
	TMap<UClass*, USystemManager*> SystemManagerMap;

	UPROPERTY()
	TMap<UClass*, ULevelManager*> LevelManagerMap;

	UPROPERTY()
	TArray<UManagerBase*> Managers;

	bool IsLevelManagerInitialized = false;
	bool IsProgressLevelManagerInitialized = false;

	UPROPERTY()
	AIslandBaseGameMode* CurrentIslandGameMode;

	UPROPERTY()
	AInGameGameMode* CurrentGameMode;

	UPROPERTY()
	ACropoutLevelScriptBase* CurrentLevelScript;

	UPROPERTY()
	APlayerController* CurrentPlayerController;

	UPROPERTY()
	AInGamePlayerController* CurrentCropoutPlayerController;


	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void InitializeSystemManagers();
	void InitializeLevelManagers();
	void ReleaseLevelManagers();

public:
	static UCropoutGameInstance* Instance;

	static UCropoutGameInstance* GetInstance()
	{
		return Instance;
	}

	template <typename Type>
	Type* GetSystemManager()
	{
		static_assert(std::is_base_of_v<USystemManager, Type>, "Type must be derived from USystemManager");
		if(SystemManagerMap.Contains(Type::StaticClass()) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("SystemManager not found: %s"), *Type::StaticClass()->GetName());
			return nullptr;
		}

		return Cast<Type>(SystemManagerMap[Type::StaticClass()]);
	}

	template <typename Type>
	Type* GetLevelManager()
	{
		static_assert(std::is_base_of_v<ULevelManager, Type>, "Type must be derived from ULevelManager");

		if(IsProgressLevelManagerInitialized == true)
		{
			UE_LOG(LogTemp, Error, TEXT("LevelManager is initializing. Please wait."));
			return nullptr;
		}

		if(IsLevelManagerInitialized == false)
		{
			InitializeLevelManagers();
		}

		if(LevelManagerMap.Contains(Type::StaticClass()) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("LevelManager not found: %s"), *Type::StaticClass()->GetName());
			return nullptr;
		}

		return Cast<Type>(LevelManagerMap[Type::StaticClass()]);
	}

	FStreamableManager& GetStreamableManager()
	{
		return streamableManager;
	}

	UGlobalSharedVariable* GetGlobalSharedVariable() const
	{
		return GlobalSharedVariable;
	}

	USaveData* GetSaveData() const
	{
		return SaveData;
	}

	void SetCurrentGameMode(AIslandBaseGameMode* gameMode);
	void SetCurrentGameMode(AInGameGameMode* gameMode);

	AIslandBaseGameMode* GetCurrentIslandGameMode() const
	{
		return CurrentIslandGameMode;
	}

	AInGameGameMode* GetCurrentGameMode() const
	{
		return CurrentGameMode;
	}

	ACropoutLevelScriptBase* GetCurrentLevelScript() const
	{
		return CurrentLevelScript;
	}

	void SetCurrentPlayerController(APlayerController* playerController);
	APlayerController* GetCurrentPlayerController();

	void SetCurrentCropoutPlayerController(AInGamePlayerController* playerController);
	AInGamePlayerController* GetCurrentCropoutPlayerController();

	void SetCurrentLevelScript(ACropoutLevelScriptBase* cropoutLevelScriptBase);
	void SystemManagerBeginDestroy(UManagerBase* Manager);
	void LevelManagerBeginDestroy(UManagerBase* Manager);
};
