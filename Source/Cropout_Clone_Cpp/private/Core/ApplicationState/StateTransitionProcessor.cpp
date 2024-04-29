// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ApplicationState/StateTransitionProcessor.h"

void UStateTransitionProcessor::AddClearPreviousStateProcess(StateProcessFunction processFunction)
{
	ClearPreviousState.Add(processFunction);
}

void UStateTransitionProcessor::AddPrepareNextStateProcess(StateProcessFunction processFunction)
{
	PrepareNextState.Add(processFunction);
}

void UStateTransitionProcessor::DoClearPreviousState()
{
	for(StateProcessFunction process : ClearPreviousState)
	{
		process();
	}
}

void UStateTransitionProcessor::DoPrepareNextState()
{
	for(StateProcessFunction process : PrepareNextState)
	{
		process();
	}
}
