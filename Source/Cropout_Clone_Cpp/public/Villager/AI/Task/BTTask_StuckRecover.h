// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StuckRecover.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_StuckRecover : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	/** CUE to play */
	UPROPERTY(Category=Node, EditAnywhere)
	FBlackboardKeySelector RecoveryPosition;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};
