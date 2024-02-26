// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SpawnData.h"
#include "Data/SpawnInstance.h"
#include "NavigationData.h"
#include "Spawner.generated.h"

class UCropoutGameInstance;

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
	FTimerHandle CheckNavMeshAndAsyncLoadTimerHandle;

	void AsyncLoadClasses();
	void AsyncLoad();
	void AsyncLoadDeferred();
	void CheckNavMeshAndAsyncLoadFinished();
	void SpawnAssets(const FSpawnData& SpawnData);
	void SpawnInstance(UInstancedStaticMeshComponent* iSMC, const float radius, const int biomeCount,
	                   const int maxSpawnCount);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnRandom();

	UPROPERTY(EditAnywhere)
	TArray<FSpawnData> SpawnTypes;

	UPROPERTY(EditAnywhere)
	TArray<FSpawnInstance> SpawnInstances;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ANavigationData> NavData;

	// Class Load
	int ClassRefIndex;
	bool AsyncLoadComplete;

	int Counter;
	int IndexCounter;
	float TotalCount = 50.f;

	bool ActorSwitch = true;
	bool AutoSpawn = false;

	FRandomStream Seed;
};
