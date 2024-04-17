// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNearestOfBuildable.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_FindNearestOfBuildable : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category="Resource", EditAnywhere)
	FBlackboardKeySelector Key_BuildableTarget;

	UPROPERTY(Category="TownHall", EditAnywhere)
	FBlackboardKeySelector Key_TownHall;

	UPROPERTY(Category="NearestTo", EditAnywhere)
	FBlackboardKeySelector Key_NearestTo;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
