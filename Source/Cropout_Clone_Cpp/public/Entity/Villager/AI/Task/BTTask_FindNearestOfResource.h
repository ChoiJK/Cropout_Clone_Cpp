// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNearestOfResource.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_FindNearestOfResource : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category="Resource", EditAnywhere)
	FBlackboardKeySelector Key_ResourceTarget;

	UPROPERTY(Category="Resource", EditAnywhere)
	FBlackboardKeySelector Key_ResourceType;

	UPROPERTY(Category="TownHall", EditAnywhere)
	FBlackboardKeySelector Key_TownHall;

	UPROPERTY(Category="NearestTo", EditAnywhere)
	FBlackboardKeySelector Key_NearestTo;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
