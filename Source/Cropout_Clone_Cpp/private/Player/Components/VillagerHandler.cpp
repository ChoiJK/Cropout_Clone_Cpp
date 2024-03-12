#include "Player/Components/VillagerHandler.h"
#include "Player/CropoutPlayer.h"

#include "Core/CropoutPlayerController.h"
#include "Villager/Villager.h"
#include "Player/Components/MovementInputHandler.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "NavigationPath.h"
#include "NavigationData.h"
#include "NavigationSystem.h"

#include "NiagaraDataInterface.h"
#include "NiagaraDataInterfaceRW.h"
#include "NiagaraDataInterfaceArray.h"
#include "NiagaraDataInterfaceArrayFloat.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Components/SphereComponent.h"

UVillagerHandler::UVillagerHandler()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imc_Villager
		(TEXT("EnhancedInput.InputMappingContext'/Game/Blueprint/Player/Input/IMC_Villager.IMC_Villager'"));
	if(imc_Villager.Succeeded())
	{
		IMC_Villager = imc_Villager.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Villager
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_Villager.IA_Villager'"));
	if(ia_Villager.Succeeded())
	{
		IA_VillagerAction = ia_Villager.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> pathEffect
		(TEXT("Niagara.NiagaraSystem'/Game/VFX/NS_Target.NS_Target'"));
	if(pathEffect.Succeeded())
	{
		NS_PathEffect = pathEffect.Object;
	}

	check(IMC_Villager != nullptr)
	check(IA_VillagerAction != nullptr)
	check(NS_PathEffect != nullptr)
}

void UVillagerHandler::Initialize()
{
	Owner = Cast<ACropoutPlayer>(GetOwner());

	check(Owner != nullptr);
}

void UVillagerHandler::BeginPlay()
{
	Super::BeginPlay();
}

void UVillagerHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(const ULocalPlayer* LocalPlayer = Owner->GetPlayerController()->GetLocalPlayer())
	{
		InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if(InputSubsystem)
		{
			InputSubsystem->AddMappingContext(IMC_Villager, 0);
			if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
			{
				EnhancedInputComponent->BindAction(IA_VillagerAction,
				                                   ETriggerEvent::Triggered,
				                                   this,
				                                   &UVillagerHandler::OnVillagerActionTriggered);

				EnhancedInputComponent->BindAction(IA_VillagerAction,
				                                   ETriggerEvent::Started,
				                                   this,
				                                   &UVillagerHandler::OnVillagerAction);

				EnhancedInputComponent->BindAction(IA_VillagerAction,
				                                   ETriggerEvent::Canceled,
				                                   this,
				                                   &UVillagerHandler::OnVillagerActionEnd);
				EnhancedInputComponent->BindAction(IA_VillagerAction,
				                                   ETriggerEvent::Completed,
				                                   this,
				                                   &UVillagerHandler::OnVillagerActionEnd);
			}
		}
	}
}

void UVillagerHandler::OnVillagerActionTriggered(const FInputActionValue& Value)
{
	VillagerActor = Owner->GetHoverActor();
}

void UVillagerHandler::OnVillagerAction(const FInputActionValue& Value)
{
	if(Owner->MovementInputHandler->SingleTouchCheck())
	{
		Owner->MovementInputHandler->PositionCheck();
	}

	AActor* selectedVillager = VillagerOverlapCheck();
	if(selectedVillager == nullptr)
	{
		Owner->MovementInputHandler->InitDragMoveIMC();
	}
	else
	{
		VillagerSelect(selectedVillager);
	}
}

void UVillagerHandler::OnVillagerActionEnd(const FInputActionValue& Value)
{
	Owner->MovementInputHandler->ReleaseDragMoveIMC();

	if(SelectedVillager && SelectedVillager->IsValidLowLevel())
	{
		Cast<AVillager>(SelectedVillager)->Action(VillagerAction);
		VillagerRelease();
	}

	VillagerAction = nullptr;
}

void UVillagerHandler::VillagerSelect(AActor* selectedVillager)
{
	SelectedVillager = selectedVillager;

	NS_PathEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS_PathEffect, Owner->GetRootComponent(), NAME_None, FVector(0.f), FRotator(0.f),
		EAttachLocation::Type::SnapToTarget, false);

	Owner->GetWorldTimerManager().SetTimer(PathTimerHandle, this, &UVillagerHandler::UpdatePath, 0.01f, true, 0.0f);
}

void UVillagerHandler::VillagerRelease()
{
	if(PathTimerHandle.IsValid())
	{
		Owner->GetWorldTimerManager().PauseTimer(PathTimerHandle);
	}

	if(NS_PathEffectComponent && NS_PathEffectComponent->IsValidLowLevel())
	{
		NS_PathEffectComponent->DestroyComponent();
	}

	SelectedVillager = nullptr;
}

AActor* UVillagerHandler::VillagerOverlapCheck()
{
	TArray<AActor*> OverlappingActors;
	FVector worldPos = Owner->Collision->GetComponentLocation();
	Owner->Collision->GetOverlappingActors(OverlappingActors, AVillager::StaticClass());

	AActor* villagerActor = nullptr;
	if(OverlappingActors.Num() > 0)
	{
		if(OverlappingActors[0] && OverlappingActors[0]->IsValidLowLevel())
		{
			villagerActor = OverlappingActors[0];
		}
	}

	return villagerActor;
}

void UVillagerHandler::UpdatePath()
{
	UNavigationSystemV1* navigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	FVector startPath = Owner->Collision->GetComponentLocation();
	FVector endPath = SelectedVillager->GetActorLocation();
	endPath.Z = startPath.Z;
	UNavigationPath* pathResult = navigationSystem->FindPathToLocationSynchronously(GetWorld(), startPath, endPath);
	if(pathResult == nullptr && pathResult->IsValid() == false)
	{
		return;
	}

	const TArray<FNavPathPoint> pathPoints = pathResult->GetPath()->GetPathPoints();

	if(pathPoints.Num() < 0)
	{
		return;
	}
	TArray<FVector> vecPathPoints;
	for(const FNavPathPoint& pathPoint : pathPoints)
	{
		vecPathPoints.Add(pathPoint.Location);
	}

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
		NS_PathEffectComponent, "TargetPath", vecPathPoints);
}


//UNavigationPath* UVillagerHandler::FindPathToLocationSynchronously(UObject* WorldContextObject,
//                                                                   const FVector& PathStart, const FVector& PathEnd)
//{
//	UWorld* World = nullptr;
//	AActor* PathfindingContext = nullptr;
//
//	if(WorldContextObject != nullptr)
//	{
//		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
//	}
//	if(World == nullptr && PathfindingContext != nullptr)
//	{
//		World = GEngine->GetWorldFromContextObject(PathfindingContext, EGetWorldErrorMode::LogAndReturnNull);
//	}
//
//	UNavigationPath* ResultPath = nullptr;
//
//	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
//
//	if(NavSys != nullptr && NavSys->GetDefaultNavDataInstance() != nullptr)
//	{
//		ResultPath = NewObject<UNavigationPath>(NavSys);
//		bool bValidPathContext = false;
//		const ANavigationData* NavigationData = nullptr;
//
//		if(PathfindingContext != nullptr)
//		{
//			INavAgentInterface* NavAgent = Cast<INavAgentInterface>(PathfindingContext);
//
//			if(NavAgent != nullptr)
//			{
//				const FNavAgentProperties& AgentProps = NavAgent->GetNavAgentPropertiesRef();
//				NavigationData = NavSys->GetNavDataForProps(AgentProps, PathStart);
//				bValidPathContext = true;
//			}
//			else if(Cast<ANavigationData>(PathfindingContext))
//			{
//				NavigationData = static_cast<ANavigationData*>(PathfindingContext);
//				bValidPathContext = true;
//			}
//		}
//		if(bValidPathContext == false)
//		{
//			// just use default
//			NavigationData = NavSys->GetDefaultNavDataInstance();
//		}
//
//		check(NavigationData);
//
//		const FPathFindingQuery Query(PathfindingContext, *NavigationData, PathStart, PathEnd,
//		                              UNavigationQueryFilter::GetQueryFilter(
//			                              *NavigationData, PathfindingContext, nullptr
//		                              ));
//		const FPathFindingResult Result = NavSys->FindPathSync(Query, EPathFindingMode::Regular);
//		if(Result.IsSuccessful())
//		{
//			ResultPath->SetPath(Result.Path);
//		}
//	}
//
//	return ResultPath;
//}
