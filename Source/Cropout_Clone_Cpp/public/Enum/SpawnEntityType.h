#pragma once

#include "CoreMinimal.h"
#include "SpawnEntityType.generated.h"

UENUM()
enum class ESpawnEntityType : uint8
{
	None UMETA(DisplayName = "None"),
	Interactable UMETA(DisplayName = "Interactable"),
	Decoration UMETA(DisplayName = "Decoration")
};
