// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ApplicationState/StateMachineBase.h"
#include "StateTransitionProcessor.generated.h"

class UDependentProcessQueue;
class UStateProcessSharedData;

UCLASS()
class CROPOUT_CLONE_CPP_API UStateTransitionProcessor : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UDependentProcessQueue*> TransitionProcessQueue;

	StateProcessFunction OnAllProcessesCompleted;

	int CurrentProcessIndex = 0;
	bool IsTransitionSuccess = false;

public:
	void AddTransitionProcess(UDependentProcessQueue* processQueue);
	void SetOnAllProcessesCompleted(const StateProcessFunction& onAllProcessesCompleted);
	void Init();
	void Process(UStateProcessSharedData* sharedData);
	bool IsSuccess() const;
};
