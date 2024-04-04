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

inline FString EnumToString(EResourceType Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);
	if(!EnumPtr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetNameStringByIndex(static_cast<uint8>(Value));
}
