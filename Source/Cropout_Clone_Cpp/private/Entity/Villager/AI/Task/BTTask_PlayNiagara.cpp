// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Villager/AI/Task/BTTask_PlayNiagara.h"
#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_PlayNiagara)

UBTTask_PlayNiagara::UBTTask_PlayNiagara(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "PlayNiagara";
}

EBTNodeResult::Type UBTTask_PlayNiagara::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();

	UNiagaraComponent* NC = nullptr;
	if(System && MyController)
	{
		if(const APawn* MyPawn = MyController->GetPawn())
		{
			NC = UNiagaraFunctionLibrary::SpawnSystemAttached(System, MyPawn->GetRootComponent(), NAME_None,
			                                                  FVector::ZeroVector, FRotator::ZeroRotator,
			                                                  EAttachLocation::KeepRelativeOffset, false);
		}
	}
	return NC ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

FString UBTTask_PlayNiagara::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: '%s'"), *Super::GetStaticDescription(),
	                       System ? *System->GetName() : TEXT(""));
}

#if WITH_EDITOR

FName UBTTask_PlayNiagara::GetNodeIconName() const
{
	return FName("AnimViewportMenu.PlayBackSpeed");
}

#endif	// WITH_EDITOR
