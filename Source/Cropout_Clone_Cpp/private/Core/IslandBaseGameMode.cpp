// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/IslandBaseGameMode.h"
#include "Core/CropoutGameInstance.h"
#include "Spawn/Spawner.h"

#include "Kismet/GameplayStatics.h"

AIslandBaseGameMode::AIslandBaseGameMode()
{
	if(auto gameInstance = GetCropoutGameInstance())
	{
		gameInstance->SetCurrentGameMode(this);
		GlobalSharedVariable = gameInstance->GetGlobalSharedVariable();
	}
}

void AIslandBaseGameMode::StartPlay()
{
	Super::StartPlay();
}

void AIslandBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AIslandBaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(auto gameInstance = GetCropoutGameInstance())
	{
		gameInstance->SetCurrentGameMode(static_cast<AIslandBaseGameMode*>(nullptr));
	}
}


void AIslandBaseGameMode::OnIslandGenComplete()
{
	OnIslandGenCompleteInternal();
}

void AIslandBaseGameMode::OnIslandGenCompleteInternal()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		if(ASpawner* spawnerRef = GetSpawner())
		{
			spawnerRef->SpawnRandom();
		}
	}));
}

ASpawner* AIslandBaseGameMode::GetSpawner()
{
	if(SpawnerRef == nullptr)
	{
		SpawnerRef = Cast<ASpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner::StaticClass()));
		check(SpawnerRef != nullptr);
	}

	return SpawnerRef;
}

UCropoutGameInstance* AIslandBaseGameMode::GetCropoutGameInstance()
{
	return UCropoutGameInstance::GetInstance();
}
