// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateProcess_ClearSaveGame.h"
#include "StateProcess_SetGamePausedMode.generated.h"

/**
 * 
 */
UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcess_SetGamePausedMode : public UStateProcessBase
{
	GENERATED_BODY()

	bool IsPaused;

public:
	virtual void Init() override;
	virtual void Process(UStateProcessSharedData* sharedData) override;
	virtual bool IsSuccess() override;
	void SetPaused(bool bPaused);
};
