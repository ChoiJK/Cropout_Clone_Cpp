// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LevelManager.h"
#include "SoundManager.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API USoundManager : public ULevelManager
{
	GENERATED_BODY()

	UPROPERTY()
	UAudioComponent* MusicComponent = nullptr;

public:
	USoundManager();
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	void PlayMusic();
};
