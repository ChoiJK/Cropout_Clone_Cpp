// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/IslandBaseGameMode.h"
#include "Enum/ResourceType.h"
#include "SimpleRx/SimpleRxReactiveProperty.h"
#include "InGameGameMode.generated.h"

class ABuildingBaseActor;
class AVillager;

UCLASS()
class CROPOUT_CLONE_CPP_API AInGameGameMode : public AIslandBaseGameMode
{
	GENERATED_BODY()

	TMap<EResourceType, std::shared_ptr<SimpleRxReactiveProperty<int>>> ResourceBank;
	void ResourceDebugMessage();

	bool IsContinueGame = false;

protected:
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnIslandGenCompleteInternal() override;

public:
	AInGameGameMode();

	TSubclassOf<AVillager> Villager_Ref;
	TArray<AVillager*> Villagers;
	void SpawnVillagersInBegin();
	AVillager* SpawnVillager();
	int GetVillagerCount() { return Villagers.Num(); }

	ABuildingBaseActor* TownHall;
	void SpawnTownHall();

	int GetResourceAmount(EResourceType type);
	bool HasResource(EResourceType type, int amount);
	void StoreResource(EResourceType type, int amount);
	void SpendResource(EResourceType type, int amount);
	bool ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount);

	std::shared_ptr<SimpleRxReactiveProperty<int>> GetResourceRP(EResourceType type);

	void SetIsContinueGame(bool value) { IsContinueGame = value; }
};
