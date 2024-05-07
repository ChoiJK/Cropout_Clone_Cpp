// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_FindNearestOfBuildable.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutGameMode.h"
#include "Entity/EntityManager.h"
#include "Enum/ResourceType.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"


class AVillager;

UBTTask_FindNearestOfBuildable::UBTTask_FindNearestOfBuildable(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "FindNearestOfBuildable";
}

EBTNodeResult::Type UBTTask_FindNearestOfBuildable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ABuildingBaseActor* target = Cast<ABuildingBaseActor>(
		BlackboardComp->GetValueAsObject(Key_BuildableTarget.SelectedKeyName));

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::Instance;

	if(ABuildingBaseActor* townHall = gameInstance->GetCurrentGameMode()->TownHall)
	{
		BlackboardComp->SetValueAsObject(Key_TownHall.SelectedKeyName, Cast<UObject>(townHall));
	}

	// 이미 target이 있고 건축이 필요하다면 기존 target을 사용
	if(target != nullptr && IsValid(target) && target->IsRequireBuild())
	{
		return EBTNodeResult::Succeeded;
	}

	const AActor* nearestTo = Cast<AActor>(BlackboardComp->GetValueAsObject(Key_NearestTo.SelectedKeyName));

	if(nearestTo == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UEntityManager* entityManager = gameInstance->GetLevelManager<UEntityManager>();
	AActor* validFoundActor = nullptr; // 찾은 Actor 중에서 가장 가깝고, nearestTo와 경로가 만들어지는 Actor
	for(int i = 0; i < entityManager->GetRequireBuildBuildingCount(); ++i)
	{
		AActor* foundActor = Cast<AActor>(
			entityManager->FindNearestRequireBuildBuilding(nearestTo->GetActorLocation(), i));

		if(foundActor)
		{
			UNavigationPath* pathResult = UNavigationSystemV1::FindPathToActorSynchronously(
				GetWorld(), nearestTo->GetActorLocation(), foundActor, 100.f);
			if(pathResult != nullptr && pathResult->IsValid() && pathResult->PathPoints.Num() > 0)
			{
				if(pathResult->IsPartial() == false)
				{
					// 경로가 완전한 경우
					validFoundActor = foundActor;
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	if(validFoundActor != nullptr)
	{
		BlackboardComp->SetValueAsObject(Key_BuildableTarget.SelectedKeyName, validFoundActor);
		bSuccess = true;
	}

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_FindNearestOfBuildable::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
