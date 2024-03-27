#pragma once

#include "CoreMinimal.h"

#include "WidgetTableData.generated.h"

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None UMETA(DisplayName = "None"),
	UiBase UMETA(DisplayName = "UiBase"),
	InGameMain UMETA(DisplayName = "InGameMainWidget"),
	ResourceElement UMETA(DisplayName = "ResourceElementWidget"),
	PauseMenu UMETA(DisplayName = "PauseMenuWidget"),
};

USTRUCT(BlueprintType)
struct FWidgetTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EWidgetType WidgetType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;
};
