#pragma once

#include "CoreMinimal.h"
#include "WidgetType.generated.h"

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None UMETA(DisplayName = "None"),
	UiBase UMETA(DisplayName = "UiBase"),
	InGameMain UMETA(DisplayName = "InGameMainWidget"),
	ResourceElement UMETA(DisplayName = "ResourceElementWidget"),
	PauseMenu UMETA(DisplayName = "PauseMenuWidget"),
	ResultMenu UMETA(DisplayName = "ResultMenuWidget"),
	BuildOpenButton UMETA(DisplayName = "BuildOpenButtonWidget"),
	BuildPanel UMETA(DisplayName = "BuildPanelWidget"),
	BuildEntityCard UMETA(DisplayName = "BuildEntityCardWidget"),
	BuildEntityInfo UMETA(DisplayName = "BuildEntityInfoWidget"),
	ConstructionResource UMETA(DisplayName = "ConstructionResourceWidget"),
	BuildPlacement UMETA(DisplayName = "BuildPlacementWidget")
};
