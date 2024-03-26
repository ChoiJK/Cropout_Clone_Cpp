// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_DoWork.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/Interactable/Resource/ResourceBaseActor.h"
#include "Entity/VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_DoWork)

UBTTask_DoWork::UBTTask_DoWork(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "DoWork";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_DoWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	bool bSuccess = false;

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVillager* MyPawn = Cast<AVillager>(MyController->GetPawn());
	if(MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}


	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AResourceBaseActor* takeFrom = Cast<AResourceBaseActor>(
		BlackboardComp->GetValueAsObject(Key_TakeFrom.SelectedKeyName));
	AVillager* giveTo = Cast<AVillager>(BlackboardComp->GetValueAsObject(Key_Give_To.SelectedKeyName));
	if(takeFrom == nullptr || IsValid(takeFrom) == false || giveTo == nullptr || IsValid(giveTo) == false)
	{
		return EBTNodeResult::Failed;
	}

	FBTTask_DoWork_Memory* doWorkMemory = reinterpret_cast<FBTTask_DoWork_Memory*>(NodeMemory);
	doWorkMemory->GiveTo = giveTo;
	doWorkMemory->TakeFrom = takeFrom;

	takeFrom->PlayWobble(MyPawn->GetActorLocation());

	float delay = takeFrom->Interact();
	MyPawn->PlayWorkAnim(delay);
	SetNextTickTime(NodeMemory, delay);

	return EBTNodeResult::InProgress;
}

void UBTTask_DoWork::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ensure(GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory)->NextTickRemainingTime <= 0.f);

	FBTTask_DoWork_Memory* doWorkMemory = reinterpret_cast<FBTTask_DoWork_Memory*>(NodeMemory);
	AResourceBaseActor* takeFrom = doWorkMemory->TakeFrom;
	AVillager* giveTo = doWorkMemory->GiveTo;

	auto result = takeFrom->CollectResource();
	giveTo->StoreResource(result.Key, result.Value);

	// continue execution from this node
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

uint16 UBTTask_DoWork::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_DoWork_Memory);
}

EBTNodeResult::Type UBTTask_DoWork::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AResourceBaseActor* takeFrom = Cast<AResourceBaseActor>(
		BlackboardComp->GetValueAsObject(Key_TakeFrom.SelectedKeyName));
	takeFrom->EndWooble();

	return EBTNodeResult::Aborted;
}

FString UBTTask_DoWork::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
