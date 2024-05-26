// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateTransitionProcessor.h"
#include "ApplicationState/DependentProcessQueue.h"


void UStateTransitionProcessor::AddTransitionProcess(UDependentProcessQueue* processQueue)
{
	TransitionProcessQueue.Add(processQueue);
}

void UStateTransitionProcessor::SetOnAllProcessesCompleted(const StateProcessFunction& onAllProcessesCompleted)
{
	OnAllProcessesCompleted = onAllProcessesCompleted;
}

void UStateTransitionProcessor::Init()
{
	CurrentProcessIndex = 0;
	IsTransitionSuccess = false;

	for(const auto process : TransitionProcessQueue)
	{
		process->Init();
	}
}

void UStateTransitionProcessor::Process(UStateProcessSharedData* sharedData)
{
	for(int i = CurrentProcessIndex; i < TransitionProcessQueue.Num(); ++i)
	{
		if(TransitionProcessQueue[i]->IsSuccess() == false)
		{
			TransitionProcessQueue[i]->Process(sharedData);

			if(TransitionProcessQueue[i]->IsSuccess() == false)
			{
				return;
			}
		}

		CurrentProcessIndex = i;
	}

	IsTransitionSuccess = true;

	if(OnAllProcessesCompleted)
	{
		OnAllProcessesCompleted();
	}
}

bool UStateTransitionProcessor::IsSuccess() const
{
	return IsTransitionSuccess;
}
