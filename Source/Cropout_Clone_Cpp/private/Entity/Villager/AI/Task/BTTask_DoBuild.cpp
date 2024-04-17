// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_DoBuild.h"

#include "AIController.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/VIllager/Villager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_DoBuild)

UBTTask_DoBuild::UBTTask_DoBuild(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "DoBuild";
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_DoBuild::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	ABuildingBaseActor* buildable = Cast<ABuildingBaseActor>(
		BlackboardComp->GetValueAsObject(Key_Buildable.SelectedKeyName));
	UObject* uobj = BlackboardComp->GetValueAsObject(Key_Builder.SelectedKeyName);
	AVillager* builder = Cast<AVillager>(uobj);
	if(buildable == nullptr || IsValid(buildable) == false || builder == nullptr || IsValid(builder) == false)
	{
		return EBTNodeResult::Failed;
	}

	if(buildable->IsRequireBuild() == false)
	{
		return EBTNodeResult::Succeeded;
	}

	FBTTask_DoBuild_Memory* doBuildMemory = reinterpret_cast<FBTTask_DoBuild_Memory*>(NodeMemory);
	doBuildMemory->Buildable = buildable;
	doBuildMemory->Builder = builder;

	buildable->AddBuilder(builder);

	float delay = buildable->Interact();
	MyPawn->PlayWorkAnim(delay);
	SetNextTickTime(NodeMemory, 1);

	return EBTNodeResult::InProgress;
}

void UBTTask_DoBuild::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTTask_DoBuild_Memory* doBuildMemory = reinterpret_cast<FBTTask_DoBuild_Memory*>(NodeMemory);
	ABuildingBaseActor* buildable = doBuildMemory->Buildable;
	AVillager* builder = doBuildMemory->Builder;

	if(buildable == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		builder->EndWorkAnim();
		return;
	}

	if(buildable->IsRequireBuild())
	{
		// 아직 Build가 필요하다면 1초 뒤에 다시 Tick
		SetNextTickTime(NodeMemory, 1);
	}
	else
	{
		buildable->RemoveBuilder(builder);
		builder->EndWorkAnim();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_DoBuild::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_DoBuild_Memory);
}

EBTNodeResult::Type UBTTask_DoBuild::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	FBTTask_DoBuild_Memory* doBuildMemory = reinterpret_cast<FBTTask_DoBuild_Memory*>(NodeMemory);
	ABuildingBaseActor* buildable = doBuildMemory->Buildable;
	AVillager* builder = doBuildMemory->Builder;

	if(MyController == nullptr)
	{
		builder->EndWorkAnim();
		return EBTNodeResult::Failed;
	}

	buildable->RemoveBuilder(builder);
	builder->EndWorkAnim();
	return EBTNodeResult::Aborted;
}

FString UBTTask_DoBuild::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
