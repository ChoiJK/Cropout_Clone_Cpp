// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoFarming.generated.h"

class ACropBaseActor;
class AVillager;

struct FBTTask_DoFarming_Memory : public FBTTaskMemory
{
	ACropBaseActor* FarmingTarget;
	AVillager* Farmer;
};

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_DoFarming : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_FarmingTarget;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Farmer;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_IsDelivery;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
};
