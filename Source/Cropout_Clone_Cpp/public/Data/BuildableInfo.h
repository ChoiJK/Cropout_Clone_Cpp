#pragma once

#include "Engine/DataTable.h"
#include "Enum/ResourceType.h"
#include "Enum/InteractableType.h"

#include "BuildableInfo.generated.h"

class UImage;

USTRUCT(BlueprintType)
struct FConstructionCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Coat;
};

USTRUCT(BlueprintType)
struct FBuildableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractableType InteractableType; // for filtering

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> UIIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor InfoBackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConstructionCost> ConstructionCosts;
};
