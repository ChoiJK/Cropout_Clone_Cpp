// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DependentProcessQueue.generated.h"

class UStateProcessBase;
class UStateProcessSharedData;

UCLASS()
class CROPOUT_CLONE_CPP_API UDependentProcessQueue : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UStateProcessBase*> DependentProcessQueue;

	int CurrentProcessIndex = 0;
	bool IsProcessSuccess = false;

public:
	void AddDependentProcess(UStateProcessBase* process);
	void Init();
	void Process(UStateProcessSharedData* sharedData);
	bool IsSuccess() const;
};
