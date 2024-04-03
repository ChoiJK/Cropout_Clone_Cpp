#pragma once

#include "Engine/DataTable.h"
#include "Enum/SpawnEntityType.h"
#include "SpawnerInfo.generated.h"

USTRUCT()
struct FSpawnerInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnEntityType SpawnEntityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BiomeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BiomeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnPerBiome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomRotationRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleRange;
};
