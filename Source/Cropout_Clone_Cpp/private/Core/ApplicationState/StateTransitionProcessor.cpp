// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ApplicationState/StateTransitionProcessor.h"

void UStateTransitionProcessor::AddClearPreviousStateProcess(StateProcess processFunction)
{
	ClearPreviousState.Add(processFunction);
}

void UStateTransitionProcessor::AddPrepareNextStateProcess(StateProcess processFunction)
{
	PrepareNextState.Add(processFunction);
}

void UStateTransitionProcessor::DoClearPreviousState()
{
	for(StateProcess process : ClearPreviousState)
	{
		process();
	}
}

void UStateTransitionProcessor::DoPrepareNextState()
{
	for(StateProcess process : PrepareNextState)
	{
		process();
	}
}
