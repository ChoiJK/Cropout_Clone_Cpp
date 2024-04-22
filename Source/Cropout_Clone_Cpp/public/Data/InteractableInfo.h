#pragma once

#include "CoreMinimal.h"
#include "Enum/InteractableType.h"
#include "Enum/ResourceType.h"
#include "InteractableInfo.generated.h"

USTRUCT(BlueprintType)
struct FInteractableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generic")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generic")
	EInteractableType InteractableType; // for filtering

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Blend")
	bool EnableGroundBlend = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Blend")
	float OutlineDraw = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	bool RequirBuildable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int BuildTimeSec = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	TArray<TSoftObjectPtr<UStaticMesh>> MeshList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
	bool UseRandomMesh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resource Properties")
	EResourceType ResourceType = EResourceType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resource Properties")
	float ResourceAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resource Properties")
	float CollectionTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resource Properties")
	int CollectionValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crop Properties")
	float GrowthTime = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crop Properties")
	int HarvestAmount = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Info")
	float BoundGap = 0.0f;
};
