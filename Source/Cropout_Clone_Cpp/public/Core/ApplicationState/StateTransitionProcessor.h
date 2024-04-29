// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ApplicationState/StateMachineBase.h"
#include "StateTransitionProcessor.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateTransitionProcessor : public UObject
{
	GENERATED_BODY()

	TArray<StateProcessFunction> ClearPreviousState;

	TArray<StateProcessFunction> PrepareNextState;

public:
	void AddClearPreviousStateProcess(StateProcessFunction processFunction);
	void AddPrepareNextStateProcess(StateProcessFunction processFunction);
	void DoClearPreviousState();
	void DoPrepareNextState();
};
