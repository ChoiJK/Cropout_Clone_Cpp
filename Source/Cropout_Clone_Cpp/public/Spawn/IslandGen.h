// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "Math/MathFwd.h"

#include "IslandGen.generated.h"


class UDynamicMesh;
class UMaterialParameterCollection;

enum class EPlatformPerformance : uint8
{
	LowEnd = 0,
	HighEnd = 1
};

class GlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API AIslandGen : public ADynamicMeshActor
{
	GENERATED_BODY()

private:
	void GenerateIslands( bool spawnMarkers );
	void FirstStep( bool spawnMarkers );
	void SecondStep();
	void ThirdStep();

	void SetIslandSeed( int32 seed = 0 );
	void ClearSpawnPoints();

	UBlueprint* GetSpawnMarker();
	UDynamicMesh* GetDynamicMesh();
	UMaterialParameterCollection* GetLandScapeMPC();
	EPlatformPerformance GetPlatformPerformance();

	UBlueprint* SpawnMarker = nullptr;
	UMaterialParameterCollection* LandScapeMPC = nullptr;
	TWeakPtr<GlobalEventDispatcher> eventDispatcher;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction( const FTransform& Transform ) override;

public:

	UPROPERTY( EditAnywhere )
	FRandomStream Seed;

	UPROPERTY( EditAnywhere )
	float MaxSpawnDistance = 0.0f;

	UPROPERTY( EditAnywhere )
	int Islands = 15;

	UPROPERTY( EditAnywhere )
	UDynamicMesh* DynMesh = nullptr;

	UPROPERTY( EditAnywhere )
	bool PreConstruct = false;

	UPROPERTY( EditAnywhere )
	TArray<FVector3d> SpawnPoints;

	UPROPERTY( EditAnywhere )
	float Radius = 0.0f;

	UPROPERTY( EditAnywhere )
	FVector2D IslandsSize = FVector2D( 800.0f, 5000.0f );

};
