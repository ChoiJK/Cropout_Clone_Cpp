// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_FindNearestOfField.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/CropoutGameInstance.h"
#include "../../../../../public/Core/InGame/InGameGameMode.h"
#include "Entity/EntityManager.h"
#include "Entity/Interactable/Crop/CropBaseActor.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Entity/VIllager/Villager.h"


UBTTask_FindNearestOfField::UBTTask_FindNearestOfField(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "FindNearestOfField";
}

EBTNodeResult::Type UBTTask_FindNearestOfField::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

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
	ACropBaseActor* target = Cast<ACropBaseActor>(
		BlackboardComp->GetValueAsObject(Key_FieldTarget.SelectedKeyName));

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::Instance;

	if(ABuildingBaseActor* townHall = gameInstance->GetCurrentGameMode()->TownHall)
	{
		BlackboardComp->SetValueAsObject(Key_TownHall.SelectedKeyName, Cast<UObject>(townHall));
	}

	if(target != nullptr && IsValid(target))
	{
		return EBTNodeResult::Succeeded;
	}

	const AActor* nearestTo = Cast<AActor>(BlackboardComp->GetValueAsObject(Key_NearestTo.SelectedKeyName));

	if(nearestTo == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UEntityManager* entityManager = gameInstance->GetLevelManager<UEntityManager>();
	ACropBaseActor* validFoundActor = nullptr; // 찾은 Actor 중에서 가장 가깝고, nearestTo와 경로가 만들어지는 Actor
	// 먼저 추수가 필요한 Crop을 찾음
	for(int i = 0; i < entityManager->GetRequireHarvestCropCount(); ++i)
	{
		ACropBaseActor* foundActor =
			entityManager->FindNearestRequireHarvestCrop(nearestTo->GetActorLocation(), i);

		// 이미 누군가가 수확하고 있는 경우는 제외
		if(foundActor && foundActor->HasFarmer() == false)
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
	}

	// 추수가 필요한 Crop이 없는 경우
	if(validFoundActor == nullptr)
	{
		// 성장시켜야하는 Crop을 찾음
		for(int i = 0; i < entityManager->GetRequirePlantCropCount(); ++i)
		{
			ACropBaseActor* foundActor =
				entityManager->FindNearestRequirePlantCrop(nearestTo->GetActorLocation(), i);

			// 이미 누군가가 성장시키고 있거나 Action이후 성장중인 경우는 제외
			if(foundActor && ((foundActor->HasFarmer() || foundActor->IsGrowth()) == false))
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
		}
	}

	if(validFoundActor != nullptr)
	{
		BlackboardComp->SetValueAsObject(Key_FieldTarget.SelectedKeyName, validFoundActor);
		validFoundActor->AssignFarmer(Cast<AVillager>(MyPawn));
	}
	else
	{
		BlackboardComp->SetValueAsObject(Key_FieldTarget.SelectedKeyName, nullptr);
	}

	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindNearestOfField::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
