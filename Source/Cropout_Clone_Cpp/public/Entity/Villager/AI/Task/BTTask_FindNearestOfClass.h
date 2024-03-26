// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNearestOfClass.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_FindNearestOfClass : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category="Class", EditAnywhere)
	bool UseBlackboardClass;
	UPROPERTY(Category="Class", EditAnywhere)
	FBlackboardKeySelector Key_TargetClass;
	UPROPERTY(Category="Class", EditAnywhere)
	UClass* ManualClass;

	UPROPERTY(Category="Tag", EditAnywhere)
	bool UseBlackboardTag;
	UPROPERTY(Category="Tag", EditAnywhere)
	FBlackboardKeySelector Key_BlackboardTag;
	UPROPERTY(Category="Tag", EditAnywhere)
	FName TagFilter;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Target;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_NearestTo;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
