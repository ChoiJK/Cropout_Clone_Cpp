// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Base/LevelManager.h"
#include "Enum/ResourceType.h"
#include "EntityManager.generated.h"

class AInteractableBaseActor;
class UGlobalSharedVariable;
class ABuildingBaseActor;
class AResourceBaseActor;
class ACropBaseActor;
class AVillager;
class ASpawner;
class FDelegateHandle;

template <typename T>
class SimpleRxReactiveProperty;

UCLASS()
class CROPOUT_CLONE_CPP_API UEntityManager : public ULevelManager
{
	GENERATED_BODY()

	UPROPERTY()
	ASpawner* SpawnerRef;
	UGlobalSharedVariable* GlobalSharedVariable;

	UPROPERTY()
	TArray<AInteractableBaseActor*> Interactables;
	TArray<ABuildingBaseActor*> Buildings;
	TArray<ABuildingBaseActor*> BuildingsRequireBuild;
	int LastSortedBuildingFrameCount = 0;
	FVector LastSortedBuildingLocation;

	TArray<AResourceBaseActor*> Resources;
	TMap<EResourceType, TArray<AResourceBaseActor*>> ResourcesByType;
	TMap<EResourceType, TTuple<int, FVector>> LastSortedResourceFrameCount;

	TArray<ACropBaseActor*> Crops;
	TArray<ACropBaseActor*> CropsRequireHarvest;
	int LastSortedRequireHarvestFrameCount = 0;
	FVector LastSortedRequireHarvestLocation;

	TArray<ACropBaseActor*> CropsRequirePlant;
	int LastSortedRequirePlantFrameCount = 0;
	FVector LastSortedRequirePlantLocation;

	UPROPERTY()
	TArray<AVillager*> Villagers;
	std::shared_ptr<SimpleRxReactiveProperty<int>> VillagerCountRp;
	void UpdateVillagerCount();

	void AddInteractable(AInteractableBaseActor* Interactable);
	void RemoveInteractable(AInteractableBaseActor* Interactable);

	FDelegateHandle IslandGenCompleteHandle;

	void OnIslandGenCompleteInternal();

public:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	virtual void OnSaveData(UInGameSaveData* SaveData) override;
	virtual void OnLoadData(UInGameSaveData* SaveData) override;

	ASpawner* GetSpawner();
	void OnIslandGenComplete();

	void AddBuilding(ABuildingBaseActor* Building, bool bRequireBuild = true);
	void RemoveBuilding(ABuildingBaseActor* Building);
	void ChangeBuildState(ABuildingBaseActor* Building, bool bRequireBuild);
	int GetRequireBuildBuildingCount();
	ABuildingBaseActor* FindNearestRequireBuildBuilding(FVector Location, int selectionIndex);

	void AddResource(AResourceBaseActor* Resource);
	void RemoveResource(AResourceBaseActor* Resource);
	int GetResourceOfTypeCount(EResourceType ResourceType);
	AResourceBaseActor* FindNearestResourceOfType(EResourceType ResourceType, FVector Location, int selectionIndex);

	void AddCrop(ACropBaseActor* Crop);
	void RemoveCrop(ACropBaseActor* Crop);
	void ChangeCropState(ACropBaseActor* Crop, bool bRequireHarvest, bool bRequirePlant);
	int GetRequireHarvestCropCount();
	int GetRequirePlantCropCount();
	ACropBaseActor* FindNearestRequireHarvestCrop(FVector Location, int selectionIndex);
	ACropBaseActor* FindNearestRequirePlantCrop(FVector Location, int selectionIndex);

	void AddVillager(AVillager* Villager);
	void RemoveVillager(AVillager* Villager);
	std::shared_ptr<SimpleRxReactiveProperty<int>> GetVillagerCountRp();

	TSubclassOf<AVillager> Villager_Ref;
	void SpawnVillagersInBegin();
	AVillager* SpawnVillager();
	int GetVillagerCount() { return Villagers.Num(); }

	ABuildingBaseActor* TownHall;
	void SpawnTownHall();
};
