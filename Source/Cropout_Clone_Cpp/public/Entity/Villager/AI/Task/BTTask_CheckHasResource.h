// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckHasResource.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_CheckHasResource : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
