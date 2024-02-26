// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CropoutGameMode.generated.h"

/**
 *
 */

class UCropoutGameInstance;
class FGlobalEventDispatcher;
class ASpawner;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TWeakPtr<FGlobalEventDispatcher> eventDispatcher;

private:
	FDelegateHandle islandGenCompleteHandle;

	void BeginAsyncSpawning();

protected:
	void OnIslandGenComplete();

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UCropoutGameInstance* GetGameInstance();

public:
	UCropoutGameInstance* GameInstance;
	ASpawner* SpawnerRef;
};
