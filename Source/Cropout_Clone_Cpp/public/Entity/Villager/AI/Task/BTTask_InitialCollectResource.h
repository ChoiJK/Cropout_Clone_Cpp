// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InitialCollectResource.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_InitialCollectResource : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category="Target Classes", EditAnywhere)
	FBlackboardKeySelector Key_ResourceClass;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Resource;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_ResourceTag;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_CollectionTarget;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_CollectionClass;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_TownHall;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
