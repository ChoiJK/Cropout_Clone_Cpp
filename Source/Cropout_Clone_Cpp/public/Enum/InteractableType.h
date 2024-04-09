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

inline FString EnumToString(EInteractableType Value)
{
	const UEnum* EnumPtr = StaticEnum<EInteractableType>();
	if(!EnumPtr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetNameStringByIndex(static_cast<uint8>(Value));
}
