// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ApplicationState/StateMachineBase.h"
#include "StateUpdater.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateUpdater : public UObject
{
	GENERATED_BODY()

	StateProcessFunction ProcessUpdate;

public:
	void SetProcessUpdate(const StateProcessFunction& processFunction);
	void DoUpdate() const;
};
