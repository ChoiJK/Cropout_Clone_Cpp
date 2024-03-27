// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Engine/StreamableManager.h"
#include "Data/SaveData.h"

#include "CropoutGameInstance.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UCropoutGameInstance : public UGameInstance
{
	GENERATED_BODY()

	USaveData* SaveData;

	TSharedPtr<FGlobalEventDispatcher> GlobalEventDispatcher;
	TSharedPtr<FGlobalSharedVariable> GlobalSharedVariable;
	FStreamableManager streamableManager;

	virtual void Init() override;
	virtual void Shutdown() override;

public:
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
};
