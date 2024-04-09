// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SpawnData.h"
#include "Data/SpawnInstance.h"
#include "NavigationData.h"
#include "Spawner.generated.h"

class UCropoutGameInstance;
class UTableManager;

struct FSpawnerInfo;
struct FInteractableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UCropoutGameInstance* GameInstance;
	UTableManager* TableManager;
	FTimerHandle CheckNavMeshAndAsyncLoadTimerHandle;

	TArray<const FSpawnerInfo*> SpawnerInfos;
	int InteractableSpawnCount = 0;
	int DecorationSpawnCount = 0;

	void CheckNavMeshAndAsyncLoadFinished();
	void SpawnAssets(const FSpawnerInfo* SpawnData, const FInteractableInfo* InteractableInfo);
	void SpawnInstance(UInstancedStaticMeshComponent* iSMC, const float radius, const int biomeCount,
		const int maxSpawnCount);

public:
	void SpawnRandom();
	AInteractableBaseActor* SpawnInteractable(const FInteractableInfo* InteractableInfo);

	//UPROPERTY(EditAnywhere)
	//TArray<FSpawnData> SpawnTypes;

	//UPROPERTY(EditAnywhere)
	//TArray<FSpawnInstance> SpawnInstances;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ANavigationData> NavData;

	// Class Load
	int ClassRefIndex;
	//bool AsyncLoadComplete;

	int Counter;
	int IndexCounter;
	float TotalCount = 50.f;

	bool AutoSpawn = false;

	FRandomStream Seed;
};
