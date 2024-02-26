#pragma once

#include "CoreMinimal.h"

#include "SpawnInstance.generated.h"

class UStaticMesh;

USTRUCT()
struct FSpawnInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> ClassRef;

	UPROPERTY(EditAnywhere)
	float BiomeScale;

	UPROPERTY(EditAnywhere)
	int BiomeCount;

	UPROPERTY(EditAnywhere)
	int SpawnPerBiome;
};
