#include "Data/SaveData.h"

USaveData::USaveData()
{
}

float USaveData::GetSoundMix(int index) const
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return 1.f;
	}

	return SoundMixes[index];
}

void USaveData::SetSoundMix(int index, float value)
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return;
	}

	SoundMixes[index] = value;
}
