// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enum/ResourceType.h"
#include "CropoutGameMode.generated.h"

class UCropoutGameInstance;
class FGlobalEventDispatcher;
class UGlobalSharedVariable;
class ABuildingBaseActor;
class ASpawner;
class AVillager;
class UInGameLayerWidget;
class UUiManager;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TMap<EResourceType, int> ResourceBank;
	void ResourceDebugMessage();

	UPROPERTY()
	UUiManager* UiManager = nullptr;
	UUiManager* GetUiManager();

public:
	ACropoutGameMode();

	TWeakPtr<FGlobalEventDispatcher> eventDispatcher;
	UGlobalSharedVariable* globalSharedVariable;

	FDelegateHandle islandGenCompleteHandle;

	void BeginAsyncSpawning();

	TSubclassOf<AVillager> Villager_Ref;
	TArray<AVillager*> Villagers;
	void SpawnVillagersInBegin();
	AVillager* SpawnVillager();
	int GetVillagerCount() { return Villagers.Num(); }

	ABuildingBaseActor* TownHall;
	void SpawnTownHall();

	ASpawner* Spawner = nullptr;
	ASpawner* GetSpawner();

	int GetResourceAmount(EResourceType type);
	bool HasResource(EResourceType type, int amount);
	void StoreResource(EResourceType type, int amount);
	bool ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount);

protected:
	void OnIslandGenComplete();

	UPROPERTY()
	UAudioComponent* MusicComponent = nullptr;
	void PlayMusic();

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UCropoutGameInstance* GetGameInstance();

public:
	UPROPERTY()
	UCropoutGameInstance* GameInstance;

	UPROPERTY()
	ASpawner* SpawnerRef;
};
