// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/InGame/InGameGameMode.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Global/GlobalSharedVariable.h"
#include "Global/GlobalUtilFunctions.h"
#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CropoutPlayer.h"
#include "Entity/Villager/Villager.h"
#include "Enum/ResourceType.h"
#include "Manager/TableManager.h"


#include "Spawn/Spawner.h"


AInGameGameMode::AInGameGameMode()
{
	DefaultPawnClass = ACropoutPlayer::StaticClass();
	PlayerControllerClass = AInGamePlayerController::StaticClass();

	Villager_Ref = AVillager::StaticClass();
}

void AInGameGameMode::StartPlay()
{
	Super::StartPlay();
}

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @ TODO : ScreenEffect FadeOut

	// 다음 프레임에 적용 (GameMode와 LevelScriptActor의 생성 순서 때문)
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		StoreResource(EResourceType::Food, 100);
	}));
}

void AInGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInGameGameMode::SpawnTownHall()
{
	UClass* bp_class = GlobalSharedVariable->GetSpawnMarkerClass();

	if(bp_class == nullptr)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	for(TActorIterator<AActor> It(GetWorld(), bp_class); It; ++It)
	{
		FoundActors.Add(*It);
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), bp_class, FoundActors);

	FVector spawnLocation = FVector::ZeroVector;
	if(FoundActors.Num() != 0)
	{
		spawnLocation = FGlobalUtilFunctions::SteppedPosition(
			FoundActors[FMath::RandRange(0, FoundActors.Num() - 1)]->GetActorLocation());
	}

	UTableManager* tableManager = GetCropoutGameInstance()->GetSystemManager<UTableManager>();
	const FInteractableInfo* info = tableManager->GetInteractableInfo("TownHall");
	TownHall = Cast<ABuildingBaseActor>(GetSpawner()->SpawnInteractable(info));
	if(TownHall)
	{
		TownHall->SetActorLocation(spawnLocation);
	}
}

int AInGameGameMode::GetResourceAmount(EResourceType type)
{
	return ResourceBank.Contains(type) ? ResourceBank[type]->GetValue() : -1;
}

bool AInGameGameMode::HasResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type]->GetValue() >= amount)
	{
		return true;
	}

	return false;
}

void AInGameGameMode::StoreResource(EResourceType type, int amount)
{
	if(!ResourceBank.Contains(type))
	{
		ResourceBank.Add(type, std::make_shared<SimpleRxReactiveProperty<int>>(amount));
	}
	else
	{
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() + amount);
	}
}

void AInGameGameMode::SpendResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - amount);

		if(IsContinueGame == false && ResourceBank[type]->GetValue() < 0 && type == EResourceType::Food)
		{
			UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}
}

bool AInGameGameMode::ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount)
{
	bool isWithdrawn = false;
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		isWithdrawn = true;
		withdrawnAmount = requestedAmount;
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - requestedAmount);

		if(IsContinueGame == false && ResourceBank[type]->GetValue() < 0 && type == EResourceType::Food)
		{
			UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}

	return isWithdrawn;
}

std::shared_ptr<SimpleRxReactiveProperty<int>> AInGameGameMode::GetResourceRP(EResourceType type)
{
	if(ResourceBank.Contains(type) == false)
	{
		ResourceBank.Add(type, std::make_shared<SimpleRxReactiveProperty<int>>(0));
	}

	return ResourceBank[type];
}

void AInGameGameMode::ResourceDebugMessage()
{
	if(ResourceBank.Contains(EResourceType::Food))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Food), 5.f, FColor::Green,
			FString::Printf(TEXT("Food %d"), ResourceBank[EResourceType::Food]->GetValue()));
	}

	if(ResourceBank.Contains(EResourceType::Wood))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Wood), 5.f, FColor::Red,
			FString::Printf(TEXT("Wood %d"), ResourceBank[EResourceType::Wood]->GetValue()));
	}

	if(ResourceBank.Contains(EResourceType::Stone))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Stone), 5.f, FColor::Black,
			FString::Printf(TEXT("Stone %d"), ResourceBank[EResourceType::Stone]->GetValue()));
	}
}

void AInGameGameMode::OnIslandGenCompleteInternal()
{
	UE_LOG(LogTemp, Warning, TEXT( "IslandGenComplete" ));
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		// @Todo : Check Save Bool (In Game instance)
		{
		}

		// else
		{
			SpawnTownHall();
			SpawnVillagersInBegin();

			Super::OnIslandGenCompleteInternal();
		}
	}));
}

void AInGameGameMode::SpawnVillagersInBegin()
{
	for(int i = 0; i < 3; ++i)
	{
		SpawnVillager();
	}
}


AVillager* AInGameGameMode::SpawnVillager()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Owner = Owner;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector location = FVector::ZeroVector;
	FVector boxExtent = FVector::ZeroVector;
	TownHall->GetActorBounds(false, location, boxExtent);
	float newPosDistance = FMath::Min(boxExtent.X, boxExtent.Y) * 2.f;
	FVector newPosDelta = FMath::VRand() * newPosDistance;
	location += newPosDelta;
	location.Z = 0.f;

	UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	FNavLocation NavLocation;
	NavSys->GetRandomReachablePointInRadius(location, 500.0f, NavLocation);

	auto NewPawn = GetWorld()->SpawnActor<AVillager>(*Villager_Ref, NavLocation, FRotator::ZeroRotator,
		ActorSpawnParams);

	if(NewPawn != nullptr)
	{
		if(NewPawn->Controller == NULL)
		{
			// NOTE: SpawnDefaultController ALSO calls Possess() to possess the pawn (if a controller is successfully spawned).
			NewPawn->SpawnDefaultController();
		}

		return NewPawn;
	}

	return nullptr;
}
