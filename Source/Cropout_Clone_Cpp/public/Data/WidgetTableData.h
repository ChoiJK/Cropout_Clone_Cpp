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
	BuildOpenButton UMETA(DisplayName = "BuildOpenButtonWidget"),
	BuildPanel UMETA(DisplayName = "BuildPanelWidget"),
	BuildEntityCard UMETA(DisplayName = "BuildEntityCardWidget"),
	BuildEntityInfo UMETA(DisplayName = "BuildEntityInfoWidget"),
	ConstructionResource UMETA(DisplayName = "ConstructionResourceWidget")
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
