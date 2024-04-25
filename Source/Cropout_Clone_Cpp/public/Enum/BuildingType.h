#pragma once

#include "CoreMinimal.h"
#include "BuildingType.generated.h"

UENUM()
enum class EBuildingType : uint8
{
	None UMETA(DisplayName = "None"),
	House UMETA(DisplayName = "House"),
	VictoryMonument UMETA(DisplayName = "VictoryMonument"),
};
