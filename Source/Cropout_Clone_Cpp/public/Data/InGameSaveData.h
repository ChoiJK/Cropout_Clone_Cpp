// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ResourceType.h"
#include "Enum/InteractableType.h"
#include "Enum/VillagerJobType.h"
#include "GameFramework/SaveGame.h"
#include "InGameSaveData.generated.h"

USTRUCT()
struct FSpawnEntitySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString EntityID;

	UPROPERTY()
	FName InteractableName;

	UPROPERTY()
	EInteractableType InteractableType;

	UPROPERTY()
	int MeshIndex;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector Scale;

	UPROPERTY()
	float FloatValue;

	UPROPERTY()
	int IntValue;
};

USTRUCT()
struct FTransformsSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FTransform> Transforms;
};

USTRUCT()
struct FVillagerSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector Location;

	UPROPERTY(VisibleAnywhere)
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere)
	int HairMeshIndex;

	UPROPERTY(VisibleAnywhere)
	EVillagerJobType JobType;

	UPROPERTY(VisibleAnywhere)
	FString TargetEntityID;

	UPROPERTY(VisibleAnywhere)
	EResourceType StoreResourceType;

	UPROPERTY(VisibleAnywhere)
	int StoreAmount;
};

UCLASS()
class CROPOUT_CLONE_CPP_API UInGameSaveData : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Options")
	int32 ResourceBank[static_cast<int>(EResourceType::Count)] = {0};

	UPROPERTY(VisibleAnywhere, Category = "Options")
	int32 IslandRandSeed = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<FSpawnEntitySaveData> SpawnEntityData;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FTransformsSaveData> DecorationTransforms;

	UPROPERTY(VisibleAnywhere)
	TArray<FVillagerSaveData> VillagerData;

public:
	void SetResourceBank(EResourceType type, int amount);
	int GetResourceBank(EResourceType type) const;

	void SetIslandRandSeed(int seed);
	int GetIslandRandSeed() const;

	void AddEntityData(FSpawnEntitySaveData data);
	TArray<FSpawnEntitySaveData>& GetEntityData();

	void AddDecorationTransform(FName name, FTransform transform);
	TMap<FName, FTransformsSaveData>& GetDecorationTransforms();

	void AddVillagerData(FVillagerSaveData data);
	TArray<FVillagerSaveData>& GetVillagerData();
};
