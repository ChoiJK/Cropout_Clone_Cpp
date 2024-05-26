// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/DependentProcessQueue.h"
#include "ApplicationState/StateProcess/StateProcessBase.h"

void UDependentProcessQueue::AddDependentProcess(UStateProcessBase* process)
{
	DependentProcessQueue.Add(process);
}

void UDependentProcessQueue::Init()
{
	CurrentProcessIndex = 0;
	IsProcessSuccess = false;

	for(const auto process : DependentProcessQueue)
	{
		process->Init();
	}
}

void UDependentProcessQueue::Process(UStateProcessSharedData* sharedData)
{
	for(int i = CurrentProcessIndex; i < DependentProcessQueue.Num(); ++i)
	{
		DependentProcessQueue[i]->Process(sharedData);

		// DependentProcessQueue[i]가 성공하지 않으면 무한대기상태에 빠질 수 있음
		if(DependentProcessQueue[i]->IsSuccess() == false)
		{
			return;
		}

		// 여기까지 왔다는 것 : DependentProcessQueue[i]->IsSuccess()가 true
		CurrentProcessIndex = i + 1;
	}

	IsProcessSuccess = true;
}

bool UDependentProcessQueue::IsSuccess() const
{
	return IsProcessSuccess;
}
