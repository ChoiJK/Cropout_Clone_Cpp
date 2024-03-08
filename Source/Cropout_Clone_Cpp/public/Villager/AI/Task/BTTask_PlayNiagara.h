// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayNiagara.generated.h"

class UNiagaraSystem;

UCLASS()
class CROPOUT_CLONE_CPP_API UBTTask_PlayNiagara : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	/** CUE to play */
	UPROPERTY(Category=Node, EditAnywhere)
	TObjectPtr<UNiagaraSystem> System;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};
