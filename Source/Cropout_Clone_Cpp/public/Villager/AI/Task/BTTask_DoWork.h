// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoWork.generated.h"

class AResourceBaseActor;
class AVillager;

struct FBTTask_DoWork_Memory : public FBTTaskMemory
{
	AResourceBaseActor* TakeFrom;
	AVillager* GiveTo;
};

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_DoWork : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Delay;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Give_To;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_TakeFrom;

	UPROPERTY(EditAnywhere)
	float DelayMultiplier;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
};
