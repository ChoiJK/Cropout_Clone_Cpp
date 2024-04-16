// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Base/LevelManager.h"
#include "Enum/ResourceType.h"
#include "EntityManager.generated.h"

class AInteractableBaseActor;
class ABuildingBaseActor;
class AResourceBaseActor;
class AVillager;

UCLASS()
class CROPOUT_CLONE_CPP_API UEntityManager : public ULevelManager
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AInteractableBaseActor*> Interactables;
	TArray<ABuildingBaseActor*> Buildings;
	TArray<ABuildingBaseActor*> BuildingsRequireBuild;
	int LastSortedBuildingFrameCount = 0;
	FVector LastSortedBuildingLocation;

	TArray<AResourceBaseActor*> Resources;
	TMap<EResourceType, TArray<AResourceBaseActor*>> ResourcesByType;
	TMap<EResourceType, TTuple<int, FVector>> LastSortedResourceFrameCount;

	UPROPERTY()
	TArray<AVillager*> Villagers;

	void AddInteractable(AInteractableBaseActor* Interactable);
	void RemoveInteractable(AInteractableBaseActor* Interactable);

protected:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

public:
	void AddBuilding(ABuildingBaseActor* Building, bool bRequireBuild = true);
	void RemoveBuilding(ABuildingBaseActor* Building);
	void ChangeBuildState(ABuildingBaseActor* Building, bool bRequireBuild);
	int GetRequireBuildBuildingCount();
	ABuildingBaseActor* FindNearestRequireBuildBuilding(FVector Location, int selectionIndex);

	void AddResource(AResourceBaseActor* Resource);
	void RemoveResource(AResourceBaseActor* Resource);
	int GetResourceOfTypeCount(EResourceType ResourceType);
	AResourceBaseActor* FindNearestResourceOfType(EResourceType ResourceType, FVector Location, int selectionIndex);

	void AddVillager(AVillager* Villager);
	void RemoveVillager(AVillager* Villager);
};
