// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Global/GlobalEventDispatcher.h"
#include "CropoutGameInstance.generated.h"

/**
 *
 */

UCLASS()
class CROPOUT_CLONE_CPP_API UCropoutGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TSharedPtr<GlobalEventDispatcher> globalEventDispatcher;

	void Init() override;
	void Shutdown() override;

public:
	TWeakPtr<GlobalEventDispatcher> GetGlobalEventDispatcher() const
	{
		return globalEventDispatcher;
	}

};
