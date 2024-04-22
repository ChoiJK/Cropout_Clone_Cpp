// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_DoFarming.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/Interactable/Crop/CropBaseActor.h"
#include "Entity/VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_DoFarming)

UBTTask_DoFarming::UBTTask_DoFarming(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "DoFarming";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_DoFarming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	bool bSuccess = false;

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVillager* MyPawn = Cast<AVillager>(MyController->GetPawn());
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ACropBaseActor* farmingTarget = Cast<ACropBaseActor>(
		BlackboardComp->GetValueAsObject(Key_FarmingTarget.SelectedKeyName));
	if(MyPawn == nullptr)
	{
		if(farmingTarget != nullptr)
		{
			farmingTarget->UnassignFarmer();
		}

		return EBTNodeResult::Failed;
	}


	AVillager* farmer = Cast<AVillager>(BlackboardComp->GetValueAsObject(Key_Farmer.SelectedKeyName));
	if(farmingTarget == nullptr || IsValid(farmingTarget) == false)
	{
		return EBTNodeResult::Failed;
	}

	if(farmer == nullptr || IsValid(farmer) == false)
	{
		farmingTarget->UnassignFarmer();
		return EBTNodeResult::Failed;
	}

	FBTTask_DoFarming_Memory* doFarmingMemory = reinterpret_cast<FBTTask_DoFarming_Memory*>(NodeMemory);
	doFarmingMemory->FarmingTarget = farmingTarget;
	doFarmingMemory->Farmer = farmer;

	farmingTarget->Interact();

	MyPawn->PlayWorkAnim(1);
	SetNextTickTime(NodeMemory, 1);

	return EBTNodeResult::InProgress;
}

void UBTTask_DoFarming::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ensure(GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory)->NextTickRemainingTime <= 0.f);

	FBTTask_DoFarming_Memory* doFarmingMemory = reinterpret_cast<FBTTask_DoFarming_Memory*>(NodeMemory);
	ACropBaseActor* farmingTarget = doFarmingMemory->FarmingTarget;

	if(farmingTarget == nullptr || IsValid(farmingTarget) == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(farmingTarget->IsReadyToHarvest() == false)
	{
		BlackboardComp->SetValueAsBool(Key_IsDelivery.SelectedKeyName, false);
	}
	else
	{
		AVillager* farmer = doFarmingMemory->Farmer;
		auto result = farmingTarget->CollectResource();
		farmer->StoreResource(result.Key, result.Value);
		BlackboardComp->SetValueAsBool(Key_IsDelivery.SelectedKeyName, true);
	}

	farmingTarget->UnassignFarmer();
	BlackboardComp->SetValueAsObject(Key_FarmingTarget.SelectedKeyName, nullptr);

	// continue execution from this node
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

uint16 UBTTask_DoFarming::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_DoFarming_Memory);
}

EBTNodeResult::Type UBTTask_DoFarming::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	FBTTask_DoFarming_Memory* doFarmingMemory = reinterpret_cast<FBTTask_DoFarming_Memory*>(NodeMemory);
	ACropBaseActor* farmingTarget = doFarmingMemory->FarmingTarget;

	if(farmingTarget != nullptr || IsValid(farmingTarget))
	{
		farmingTarget->UnassignFarmer();
	}

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Aborted;
}

FString UBTTask_DoFarming::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
