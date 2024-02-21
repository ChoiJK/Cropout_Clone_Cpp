// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CropoutGameMode.generated.h"

/**
 *
 */

class UGlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TWeakObjectPtr<UGlobalEventDispatcher> eventDispatcher;

private:
	FDelegateHandle islandGenCompleteHandle;


protected:
	void OnIslandGenComplete();
	void BeginPlay() override;
	void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

};
