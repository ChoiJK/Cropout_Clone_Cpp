// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateProcessBase.h"
#include "ApplicationState/StateMachineBase.h"
#include "StateProcess_ProcessLambda.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcess_ProcessLambda : public UStateProcessBase
{
	GENERATED_BODY()

	TFunction<void()> Lambda;

public:
	virtual void Init() override;
	virtual void Process(UStateProcessSharedData* sharedData) override;
	virtual bool IsSuccess() override;
	void SetLambda(const TFunction<void()>& InLambda);
};
