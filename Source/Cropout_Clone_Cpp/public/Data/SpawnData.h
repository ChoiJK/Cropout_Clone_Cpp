#pragma once

#include "CoreMinimal.h"
#include "Interactable/Resource/ResourceBaseActor.h"

#include "SpawnData.generated.h"

USTRUCT()
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AResourceBaseActor> ClassRef;

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
