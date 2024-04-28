// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ApplicationState/StateMachineBase.h"
#include "StateTransitionProcessor.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateTransitionProcessor : public UObject
{
	GENERATED_BODY()

	TArray<StateProcess> ClearPreviousState;

	TArray<StateProcess> PrepareNextState;

public:
	void AddClearPreviousStateProcess(StateProcess processFunction);
	void AddPrepareNextStateProcess(StateProcess processFunction);
	void DoClearPreviousState();
	void DoPrepareNextState();
};
