// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CropoutGameMode.generated.h"

/**
 *
 */

class GlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TWeakPtr<GlobalEventDispatcher> eventDispatcher;

private:
	FDelegateHandle islandGenCompleteHandle;


protected:
	void OnIslandGenComplete();

	void StartPlay() override;
	void BeginPlay() override;
	void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;


};
