// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoBuild.generated.h"

class ABuildingBaseActor;
class AVillager;

struct FBTTask_DoBuild_Memory : public FBTTaskMemory
{
	ABuildingBaseActor* Buildable;
	AVillager* Builder;
};

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_DoBuild : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Builder;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Buildable;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
};
