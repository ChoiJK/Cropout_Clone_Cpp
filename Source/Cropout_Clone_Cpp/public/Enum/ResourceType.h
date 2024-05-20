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
	Count UMETA(Hidden)
};

inline FString EnumToString(EResourceType Value)
{
	const UEnum* EnumPtr = StaticEnum<EResourceType>();
	if(!EnumPtr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetNameStringByIndex(static_cast<uint8>(Value));
}
