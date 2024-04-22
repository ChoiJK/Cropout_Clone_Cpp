// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_ResetIsDelivery.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_ResetIsDelivery)

UBTTask_ResetIsDelivery::UBTTask_ResetIsDelivery(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Reset IsDelivery";
}

EBTNodeResult::Type UBTTask_ResetIsDelivery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* blackboardComp = MyController->GetBlackboardComponent();

	blackboardComp->SetValueAsBool(Key_IsDelivery.SelectedKeyName, false);

	return EBTNodeResult::Succeeded;
}

FString UBTTask_ResetIsDelivery::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
