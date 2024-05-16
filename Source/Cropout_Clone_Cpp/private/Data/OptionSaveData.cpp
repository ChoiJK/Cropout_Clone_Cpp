#include "Data/OptionSaveData.h"

UOptionSaveData::UOptionSaveData()
{
}

float UOptionSaveData::GetSoundMix(int index) const
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return 1.f;
	}

	return SoundMixes[index];
}

void UOptionSaveData::SetSoundMix(int index, float value)
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return;
	}

	SoundMixes[index] = value;
}
