// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/CropoutGameMode.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Global/GlobalUtilFunctions.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CropoutPlayer.h"
#include "Spawn/Spawner.h"
#include "Entity/Villager/Villager.h"
#include "AudioModulationStatics.h"
#include "ShaderPrintParameters.h"
#include "UI/UiManager.h"
#include "Enum/ResourceType.h"

ACropoutGameMode::ACropoutGameMode()
{
	DefaultPawnClass = ACropoutPlayer::StaticClass();
	PlayerControllerClass = ACropoutPlayerController::StaticClass();

	Villager_Ref = AVillager::StaticClass();
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

	PlayMusic();

	SpawnerRef = Cast<ASpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner::StaticClass()));
	if(SpawnerRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnerRef is nullptr"));
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}

	UIManager = Cast<AUiManager>(
		GetWorld()->SpawnActor(AUiManager::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator));

	check(UIManager);

	// Save Resrouce with creation complete up to the UI
	StoreResource(EResourceType::Food, 100);
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


void ACropoutGameMode::PlayMusic()
{
	FName pianoVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Piano_Vol.Cropout_Music_Piano_Vol'");
	USoundControlBus* pianoVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *pianoVolPath.ToString()));

	FName percVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Perc_Vol.Cropout_Music_Perc_Vol'");
	USoundControlBus* percVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *percVolPath.ToString()));

	FName stringsVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Strings_Delay.Cropout_Music_Strings_Delay'");
	USoundControlBus* stringsVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *stringsVolPath.ToString()));

	FName winLosePath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_WinLose.Cropout_Music_WinLose'");
	USoundControlBus* winLoseControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *winLosePath.ToString()));

	FName musicStopPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Stop.Cropout_Music_Stop'");
	USoundControlBus* musicStopControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *musicStopPath.ToString()));

	FName newMapPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_NewMap.Cropout_Music_NewMap'");
	USoundControlBus* newMapControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *newMapPath.ToString()));

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), pianoVolControlBus, 1.f, 1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), percVolControlBus, 1.f, 1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), stringsVolControlBus, 0.f, 1.f);

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), winLoseControlBus, 0.5f, 0.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), musicStopControlBus, 0.f, 0.f);

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), newMapControlBus, 1.f);

	FName mainMusicPath = TEXT(
		"MetasoundEngine.MetaSoundSource'/Game/Audio/MUSIC/MUS_Main_MSS.MUS_Main_MSS'");
	USoundBase* mainMusic = Cast<USoundBase>(
		StaticLoadObject(USoundBase::StaticClass(), nullptr, *mainMusicPath.ToString()));
	MusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), mainMusic, 1.f, 1.f, 0.f,
	                                                 nullptr, true);

	MusicComponent->Play();
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

int ACropoutGameMode::GetResourceAmount(EResourceType type)
{
	return ResourceBank.Contains(type) ? ResourceBank[type] : 0;
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

void ACropoutGameMode::StoreResource(EResourceType type, int amount)
{
	if(!ResourceBank.Contains(type))
	{
		ResourceBank.Add(type, amount);
	}
	else
	{
		ResourceBank[type] += amount;
	}

	UIManager->UpdateResourcesWidget(type);
	//ResourceDebugMessage();
}

bool ACropoutGameMode::ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount)
{
	bool isWithdrawn = false;
	if(ResourceBank.Contains(type) && ResourceBank[type] > 0)
	{
		isWithdrawn = true;
		if(ResourceBank[type] <= requestedAmount)
		{
			withdrawnAmount = ResourceBank[type];
			ResourceBank[type] = 0;
		}
		else
		{
			withdrawnAmount = requestedAmount;
			ResourceBank[type] -= requestedAmount;
		}
	}

	UIManager->UpdateResourcesWidget(type);
	//ResourceDebugMessage();

	return isWithdrawn;
}

void ACropoutGameMode::ResourceDebugMessage()
{
	if(ResourceBank.Contains(EResourceType::Food))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Food), 5.f, FColor::Green,
		                                 FString::Printf(TEXT("Food %d"), ResourceBank[EResourceType::Food]));
	}

	if(ResourceBank.Contains(EResourceType::Wood))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Wood), 5.f, FColor::Red,
		                                 FString::Printf(TEXT("Wood %d"), ResourceBank[EResourceType::Wood]));
	}

	if(ResourceBank.Contains(EResourceType::Stone))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Stone), 5.f, FColor::Black,
		                                 FString::Printf(TEXT("Stone %d"), ResourceBank[EResourceType::Stone]));
	}
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
			SpawnVillagersInBegin();

			SpawnerRef->SpawnRandom();
		}
	}));
}

void ACropoutGameMode::SpawnVillagersInBegin()
{
	for(int i = 0; i < 3; ++i)
	{
		SpawnVillager();
	}
}


AVillager* ACropoutGameMode::SpawnVillager()
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

		if(NewPawn != nullptr)
		{
			Villagers.Add(NewPawn);
			UIManager->UpdateVillagerCount();
		}

		return NewPawn;
	}

	return nullptr;
}
