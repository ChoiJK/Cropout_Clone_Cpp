// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/CropoutGameMode.h"

#include "../../public/Global/GlobalEventDispatcher.h"
#include "Core/CropoutGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/Spawner.h"

void ACropoutGameMode::StartPlay()
{
	if(auto gameInstance = GetGameInstance())
	{
		eventDispatcher = gameInstance->GetGlobalEventDispatcher();
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
			// @Todo : Spawn Town Hall
			// @Todo : Spawn Villager

			SpawnerRef->SpawnRandom();
		}
	}));
}
