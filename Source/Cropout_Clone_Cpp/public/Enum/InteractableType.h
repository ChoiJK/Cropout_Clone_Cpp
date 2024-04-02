#pragma once

#include "CoreMinimal.h"
#include "InteractableType.generated.h"

UENUM()
enum class EInteractableType : uint8
{
	None UMETA(DisplayName = "None"),
	Resource UMETA(DisplayName = "Resource"),
	Building UMETA(DisplayName = "Building"),
	Field UMETA(DisplayName = "Field"),
};
