#pragma once

#include "CoreMinimal.h"
#include "SoundMixType.generated.h"

UENUM()
enum class ESoundMixType : uint8
{
	Music = 0 UMETA(DisplayName = "Music"),
	SFX = 1 UMETA(DisplayName = "SFX")
};
