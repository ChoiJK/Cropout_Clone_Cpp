#pragma once
#include "CoreMinimal.h"
#include "Enum/SoundMixType.h"
#include "SoundMixClassInfo.generated.h"

USTRUCT(BlueprintType)
struct FSoundMixClassInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoundMixType Index;

	UPROPERTY(EditAnywhere)
	USoundClass* InSoundClass;

	UPROPERTY(EditAnywhere)
	USoundMix* InSoundMixModifier;
};
