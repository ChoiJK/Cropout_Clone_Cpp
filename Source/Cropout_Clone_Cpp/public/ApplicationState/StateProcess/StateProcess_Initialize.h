// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateProcessBase.h"
#include "StateProcess_Initialize.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcess_Initialize : public UStateProcessBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Process(UStateProcessSharedData* sharedData) override;
	virtual bool IsSuccess() override;
};
