// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateProcessBase.h"
#include "StateProcess_OpenLevel.generated.h"

/**
 * 
 */
UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcess_OpenLevel : public UStateProcessBase
{
	GENERATED_BODY()

	bool isOpenProcess = false;
	bool isOpenSuccess = false;
	FString LevelName;

protected:
	UFUNCTION()
	void OnPostLoadMapWithWorld(UWorld* LoadedWorld);
	FDelegateHandle OnPostLoadMapWithWorldDelegateHandle;

public:
	virtual void Init() override;
	virtual void Process(UStateProcessSharedData* sharedData) override;
	virtual bool IsSuccess() override;

	void SetLevelName(const FString& String);
	void OpenLevel();
};
