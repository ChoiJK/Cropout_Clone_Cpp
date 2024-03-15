// Fill out your copyright notice in the Description page of Project Settings.


#include "Villager/AI/Task/BTTask_DeliverResource.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_DeliverResource)

UBTTask_DeliverResource::UBTTask_DeliverResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Deliver Resource";
}

EBTNodeResult::Type UBTTask_DeliverResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const UBlackboardComponent* blackboardComp = MyController->GetBlackboardComponent();

	AVillager* takeFrom = Cast<AVillager>(blackboardComp->GetValueAsObject(TakeFrom.SelectedKeyName));
	takeFrom->DeliverResource();

	return EBTNodeResult::Succeeded;
}

FString UBTTask_DeliverResource::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
