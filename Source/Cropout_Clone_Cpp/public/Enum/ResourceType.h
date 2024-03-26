#pragma once

#include "CoreMinimal.h"
#include "ResourceType.generated.h"

UENUM()
enum class EResourceType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
};
