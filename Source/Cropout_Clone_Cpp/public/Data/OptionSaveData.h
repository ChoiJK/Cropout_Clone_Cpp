#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionSaveData.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UOptionSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UOptionSaveData();

private:
	float SoundMixes[2] = {1.f, 1.f};

public:
	float GetSoundMix(int index) const;
	void SetSoundMix(int index, float value);
};
