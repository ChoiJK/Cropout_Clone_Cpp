// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Engine/StreamableManager.h"
#include "Manager/TableManager.h"
#include "Data/SaveData.h"
#include "Level/CropoutLevelScriptBase.h"

#include "CropoutGameInstance.generated.h"

class ACropoutGameMode;
class ACropoutLevelScriptBase;
class USystemManager;

UCLASS()
class CROPOUT_CLONE_CPP_API UCropoutGameInstance : public UGameInstance
{
	GENERATED_BODY()

	USaveData* SaveData;

	TSharedPtr<FGlobalEventDispatcher> GlobalEventDispatcher;
	TSharedPtr<FGlobalSharedVariable> GlobalSharedVariable;
	FStreamableManager streamableManager;

	TMap<UClass*, USystemManager*> SystemManagerMap;

	ACropoutGameMode* CurrentGameMode;
	ACropoutLevelScriptBase* CurrentLevelScript;

	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void InitializeSystemManagers();

public:
	template <typename Type>
	Type* GetSystemManager()
	{
		static_assert(std::is_base_of_v<USystemManager, Type>, "Type must be derived from ULevelManager");
		if(SystemManagerMap.Contains(Type::StaticClass()) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("SystemManager not found: %s"), *Type::StaticClass()->GetName());
			return nullptr;
		}

		return Cast<Type>(SystemManagerMap[Type::StaticClass()]);
	}

	FStreamableManager& GetStreamableManager()
	{
		return streamableManager;
	}

	TWeakPtr<FGlobalEventDispatcher> GetGlobalEventDispatcher() const
	{
		return GlobalEventDispatcher;
	}

	TWeakPtr<FGlobalSharedVariable> GetGlobalSharedVariable() const
	{
		return GlobalSharedVariable;
	}

	USaveData* GetSaveData() const
	{
		return SaveData;
	}

	void SetCurrentGameMode(ACropoutGameMode* gameMode)
	{
		CurrentGameMode = gameMode;
	}

	ACropoutGameMode* GetCurrentGameMode() const
	{
		return CurrentGameMode;
	}

	void SetCurrentLevelScript(ACropoutLevelScriptBase* cropoutLevelScriptBase)
	{
		CurrentLevelScript = cropoutLevelScriptBase;
	}

	ACropoutLevelScriptBase* GetCurrentLevelScript() const
	{
		return CurrentLevelScript;
	}
};
