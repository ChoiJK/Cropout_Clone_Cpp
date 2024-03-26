// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_InitialCollectResource.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/CropoutGameMode.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Entity/Interactable/Resource/ResourceBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "Entity/VIllager/Villager.h"


class AVillager;

UBTTask_InitialCollectResource::UBTTask_InitialCollectResource(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "Initial Collect Resource";
}

EBTNodeResult::Type UBTTask_InitialCollectResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	bool bSuccess = false;

	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = MyController->GetPawn();
	if(MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if(MyPawn->Tags.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	TArray<AActor*> ResourceActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AResourceBaseActor::StaticClass(), MyPawn->Tags[0],
	                                             ResourceActors);
	if(ResourceActors.Num() > 0)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		BlackboardComp->SetValueAsName(Key_ResourceTag.SelectedKeyName, MyPawn->Tags[0]);
		BlackboardComp->SetValueAsClass(Key_ResourceClass.SelectedKeyName, AResourceBaseActor::StaticClass());

		AVillager* villager = Cast<AVillager>(MyPawn);
		AActor* targetRef = villager->GetTargetRef();
		// valid check 이유 : 최초 설정된 target을 모두 채취해서 없어졌을 때 nullptr로 설정하기 위함.
		if(targetRef != nullptr && targetRef->IsValidLowLevel() == false)
		{
			targetRef = nullptr;
		}
		BlackboardComp->SetValueAsObject(Key_Resource.SelectedKeyName, targetRef);

		AActor* TownHallActor = Cast<ACropoutGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->
			TownHall;
		if(TownHallActor != nullptr)
		{
			BlackboardComp->SetValueAsObject(Key_TownHall.SelectedKeyName, TownHallActor);
			BlackboardComp->SetValueAsObject(Key_CollectionTarget.SelectedKeyName, TownHallActor);
			BlackboardComp->SetValueAsClass(Key_CollectionClass.SelectedKeyName, TownHallActor->GetClass());
			bSuccess = true;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_InitialCollectResource::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
