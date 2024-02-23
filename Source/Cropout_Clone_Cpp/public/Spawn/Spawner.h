// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SpawnData.h"
#include "Spawner.generated.h"

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
	void LoadClasses();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnRandom();

	UPROPERTY(EditAnywhere)
	TArray<FSpawnData> SpawnTypes;

	// Class Load
	int ClassRefIndex;


	int Counter;
	int IndexCounter;
};
