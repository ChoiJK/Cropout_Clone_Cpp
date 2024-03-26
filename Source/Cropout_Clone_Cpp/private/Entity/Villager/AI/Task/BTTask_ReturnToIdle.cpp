// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_ReturnToIdle.h"

#include "AIController.h"
#include "Entity/VIllager/Villager.h"


UBTTask_ReturnToIdle::UBTTask_ReturnToIdle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "ReturnToIdle";
}

EBTNodeResult::Type UBTTask_ReturnToIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	bool bSuccess = false;
	if(MyController)
	{
		if(APawn* MyPawn = MyController->GetPawn())
		{
			AVillager* villager = Cast<AVillager>(MyPawn);
			villager->SetDefaultJob();
		}
	}
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_ReturnToIdle::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
