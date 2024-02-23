#pragma once

#include "CoreMinimal.h"

#include "SpawnData.generated.h"

class AResourceBaseActor;

USTRUCT()
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AResourceBaseActor* ClassRef;

	UPROPERTY(EditAnywhere)
	float BiomeScale;

	UPROPERTY(EditAnywhere)
	int BiomeCount;

	UPROPERTY(EditAnywhere)
	int SpawnPerBiome;

	UPROPERTY(EditAnywhere)
	float RandomRotationRange;

	UPROPERTY(EditAnywhere)
	float ScaleRange;
};
