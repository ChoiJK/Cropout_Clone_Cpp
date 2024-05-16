// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LevelManager.h"
#include "Base/SystemManager.h"
#include "SoundManager.generated.h"

class USoundClass;
class USoundMix;

UCLASS()
class CROPOUT_CLONE_CPP_API USoundManager : public USystemManager
{
	GENERATED_BODY()

	UPROPERTY()
	UAudioComponent* MusicComponent = nullptr;

	float SoundMixes[2] = {1.f, 1.f};

public:
	USoundManager();
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	void PlayMusic();
	void StopMusic();

	float GetSoundMix(int index) const;
	void SetSoundMix(int index, float value, USoundClass* InSoundClass, USoundMix* InSoundMixModifier);
};
