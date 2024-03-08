// Fill out your copyright notice in the Description page of Project Settings.


#include "Villager/AI/Task/BTTask_StuckRecover.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_StuckRecover)

UBTTask_StuckRecover::UBTTask_StuckRecover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "StuckRecover";
}

EBTNodeResult::Type UBTTask_StuckRecover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	bool bSuccess = false;
	if(MyController)
	{
		if(APawn* MyPawn = MyController->GetPawn())
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			auto Value = BlackboardComp->GetValueAsVector(RecoveryPosition.SelectedKeyName);
			Value.Z += 45.0f;
			bSuccess = MyPawn->SetActorLocation(Value);
		}
	}
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_StuckRecover::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: '%s'"), *Super::GetStaticDescription(),
	                       *RecoveryPosition.SelectedKeyName.ToString());
}

#if WITH_EDITOR

FName UBTTask_StuckRecover::GetNodeIconName() const
{
	return FName("AnimViewportMenu.PlayBackSpeed");
}

#endif	// WITH_EDITOR
