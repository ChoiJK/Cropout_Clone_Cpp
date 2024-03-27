#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API USaveData : public USaveGame
{
	GENERATED_BODY()

public:
	USaveData();

private:
	float SoundMixes[2] = {1.f, 1.f};

public:
	float GetSoundMix(int index) const;
	void SetSoundMix(int index, float value);
};
