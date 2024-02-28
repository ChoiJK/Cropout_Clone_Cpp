// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/CropoutGameMode.h"

#include "EngineUtils.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Global/GlobalUtilFunctions.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Interactable/Building/BuildingBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CropoutPlayer.h"
#include "Spawn/Spawner.h"

ACropoutGameMode::ACropoutGameMode()
{
	DefaultPawnClass = ACropoutPlayer::StaticClass();
	PlayerControllerClass = ACropoutPlayerController::StaticClass();
}

void ACropoutGameMode::StartPlay()
{
	if(auto gameInstance = GetGameInstance())
	{
		eventDispatcher = gameInstance->GetGlobalEventDispatcher();
		globalSharedVariable = gameInstance->GetGlobalSharedVariable();
	}

	if(TSharedPtr<FGlobalEventDispatcher> shared = eventDispatcher.Pin())
	{
		islandGenCompleteHandle = shared->AddListenerUObject(EGlobalEventType::IslandGenComplete, this,
		                                                     &ACropoutGameMode::OnIslandGenComplete);
		if(islandGenCompleteHandle.IsValid() == false)
		{
			UE_LOG(LogTemp, Error, TEXT( "Failed to add listener for IslandGenComplete" ));
		}
	}

	Super::StartPlay();
}

void ACropoutGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @ TODO : ScreenEffect FadeOut

	SpawnerRef = Cast<ASpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner::StaticClass()));
	if(SpawnerRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnerRef is nullptr"));
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}

	// @ TODO : Add Game HUD to screen
}

void ACropoutGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(TSharedPtr<FGlobalEventDispatcher> shared = eventDispatcher.Pin())
	{
		shared->RemoveListener(EGlobalEventType::IslandGenComplete, islandGenCompleteHandle);
	}

	Super::EndPlay(EndPlayReason);
}

UCropoutGameInstance* ACropoutGameMode::GetGameInstance()
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UCropoutGameInstance>(GetWorld()->GetGameInstance());
	}

	return GameInstance;
}

void ACropoutGameMode::SpawnTownHall()
{
	auto townHallRef = GetTownHallRef();

	if(townHallRef == nullptr)
	{
		return;
	}

	UClass* bp_class = nullptr;
	if(TSharedPtr<FGlobalSharedVariable> gsv = globalSharedVariable.Pin())
	{
		bp_class = gsv->GetSpawnMarkerClass();
	}

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

	TownHall = Cast<ABuildingBaseActor>(GetWorld()->SpawnActor(townHallRef, &spawnLocation));
}


TSubclassOf<ABuildingBaseActor> ACropoutGameMode::GetTownHallRef()
{
	if(TownHall_Ref == nullptr)
	{
		FName path = TEXT("Blueprint'/Game/Blueprint/Interactable/Building/BP_TownCenter.BP_TownCenter'");
		auto townHall_BP = Cast<UBlueprint>(
			StaticLoadObject(UBlueprint::StaticClass(), nullptr, *path.ToString()));

		TownHall_Ref = townHall_BP->GeneratedClass;
	}

	return TownHall_Ref;
}


void ACropoutGameMode::OnIslandGenComplete()
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
			// @Todo : Spawn Villager

			SpawnerRef->SpawnRandom();
		}
	}));
}
