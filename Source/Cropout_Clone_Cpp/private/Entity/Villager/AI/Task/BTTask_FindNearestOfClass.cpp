// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_FindNearestOfClass.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


class AVillager;

UBTTask_FindNearestOfClass::UBTTask_FindNearestOfClass(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "Find Closest Resource";
}

EBTNodeResult::Type UBTTask_FindNearestOfClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AActor* target = Cast<AActor>(BlackboardComp->GetValueAsObject(Key_Target.SelectedKeyName));
	// 이미 target이 있다면 그것을 사용함.
	if(target != nullptr && target->IsValidLowLevel())
	{
		return EBTNodeResult::Succeeded;
	}

	AActor* nearestTo = Cast<AActor>(BlackboardComp->GetValueAsObject(Key_NearestTo.SelectedKeyName));

	if(nearestTo == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FName targetTag = UseBlackboardTag
		                  ? BlackboardComp->GetValueAsName(Key_BlackboardTag.SelectedKeyName)
		                  : TagFilter;
	UClass* targetClass = UseBlackboardClass
		                      ? BlackboardComp->GetValueAsClass(Key_TargetClass.SelectedKeyName)
		                      : ManualClass;
	TArray<AActor*> foundActors;
	if(targetTag.IsNone())
	{
		// Target Class를 이용한 검색
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), targetClass, foundActors);
	}
	else
	{
		// Target Class && targetTag 이용한 검색
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), targetClass, targetTag, foundActors);
	}

	if(foundActors.IsEmpty())
	{
		return EBTNodeResult::Failed;
	}

	// 최적화할 건덕지가 많을듯
	// foundActors 거리순 정렬
	// Map으로 해보자
	TSortedMap<float, int> sortedMap;
	for(int i = 0; i < foundActors.Num(); i++)
	{
		AActor* foundActor = foundActors[i];
		float dist = FVector::DistSquaredXY(foundActor->GetActorLocation(), nearestTo->GetActorLocation());
		sortedMap.Add(dist, i);
	}
	//foundActors.Sort([nearestTo](const AActor& A, const AActor& B)
	//{
	//	return FVector::DistSquared(A.GetActorLocation(), nearestTo->GetActorLocation()) <
	//		FVector::DistSquared(B.GetActorLocation(), nearestTo->GetActorLocation());
	//});

	AActor* validFoundActor = nullptr; // 찾은 Actor 중에서 가장 가깝고, nearestTo와 경로가 만들어지는 Actor
	// @TODO : 채취 후 TownHall로 진입가능한지에 대한 검사를 추가해야할 것 같음(게임 플레이상 문제 없다면 패스)
	for(auto& pair : sortedMap)
	{
		AActor* foundActor = foundActors[pair.Value];
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

	if(validFoundActor != nullptr)
	{
		BlackboardComp->SetValueAsObject(Key_Target.SelectedKeyName, validFoundActor);
		bSuccess = true;
	}

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_FindNearestOfClass::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s"), *Super::GetStaticDescription());
}
