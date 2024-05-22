// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_CheckHasResource.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_CheckHasResource)

UBTTask_CheckHasResource::UBTTask_CheckHasResource(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "Check Has Resource";
}

EBTNodeResult::Type UBTTask_CheckHasResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVillager* MyPawn = Cast<AVillager>(MyController->GetPawn());
	if(MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if(MyPawn->CheckHasResource())
	{
		// Resource가 있을 때 Sequence를 다음 Sequence로 넘김
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

FString UBTTask_CheckHasResource::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
