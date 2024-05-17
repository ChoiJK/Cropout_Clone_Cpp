#pragma once

#include "CoreMinimal.h"
#include "SoundMixType.generated.h"

UENUM()
enum class ESoundMixType : uint8
{
	Music = 0 UMETA(DisplayName = "Music"),
	SFX = 1 UMETA(DisplayName = "SFX")
};

inline FString EnumToString(ESoundMixType Value)
{
	const UEnum* EnumPtr = StaticEnum<ESoundMixType>();
	if(!EnumPtr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetNameStringByIndex(static_cast<uint8>(Value));
}
