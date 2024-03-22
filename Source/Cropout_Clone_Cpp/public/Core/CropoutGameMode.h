// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interactable/Resource/ResourceBaseActor.h"
#include "CropoutGameMode.generated.h"

class UCropoutGameInstance;
class FGlobalEventDispatcher;
class FGlobalSharedVariable;
class ABuildingBaseActor;
class ASpawner;
class AVillager;
class UInGameLayerWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TMap<EResourceType, int> ResourceBank;
	void UpdateResourcesWidget(EResourceType type);
	void ResourceDebugMessage();

public:
	ACropoutGameMode();

	TWeakPtr<FGlobalEventDispatcher> eventDispatcher;
	TWeakPtr<FGlobalSharedVariable> globalSharedVariable;

	FDelegateHandle islandGenCompleteHandle;

	void BeginAsyncSpawning();

	TSubclassOf<AVillager> Villager_Ref;
	TArray<AVillager*> Villagers;
	void SpawnVillagers();
	AVillager* SpawnVillager();

	ABuildingBaseActor* TownHall;
	TSubclassOf<ABuildingBaseActor> TownHall_Ref;
	TSubclassOf<ABuildingBaseActor> GetTownHallRef();
	void SpawnTownHall();

	void StoreResource(EResourceType type, int amount);
	bool ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount);

protected:
	void OnIslandGenComplete();

	UAudioComponent* MusicComponent = nullptr;
	void PlayMusic();

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UCropoutGameInstance* GetGameInstance();

public:
	UCropoutGameInstance* GameInstance;
	ASpawner* SpawnerRef;
	UInGameLayerWidget* UI_HUD;
};
