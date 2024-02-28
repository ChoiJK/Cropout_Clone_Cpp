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
class FGlobalSharedVariable;
class ABuildingBaseActor;
class ASpawner;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACropoutGameMode();

	TWeakPtr<FGlobalEventDispatcher> eventDispatcher;
	TWeakPtr<FGlobalSharedVariable> globalSharedVariable;

	FDelegateHandle islandGenCompleteHandle;

	void BeginAsyncSpawning();

	ABuildingBaseActor* TownHall;
	TSubclassOf<ABuildingBaseActor> TownHall_Ref;
	TSubclassOf<ABuildingBaseActor> GetTownHallRef();
	void SpawnTownHall();

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
