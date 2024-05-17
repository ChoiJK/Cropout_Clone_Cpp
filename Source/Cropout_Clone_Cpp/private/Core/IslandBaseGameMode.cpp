// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/IslandBaseGameMode.h"
#include "Core/CropoutGameInstance.h"
#include "Manager/SoundManager.h"

AIslandBaseGameMode::AIslandBaseGameMode()
{
	if(auto gameInstance = GetCropoutGameInstance())
	{
		gameInstance->SetCurrentGameMode(this);
	}
}

void AIslandBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(auto gameInstance = GetCropoutGameInstance())
	{
		USoundManager* soundManager = gameInstance->GetSystemManager<USoundManager>();
		soundManager->PlayMusic();
	}
}

void AIslandBaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(auto gameInstance = GetCropoutGameInstance())
	{
		gameInstance->SetCurrentGameMode(static_cast<AIslandBaseGameMode*>(nullptr));
	}
}

UCropoutGameInstance* AIslandBaseGameMode::GetCropoutGameInstance()
{
	return UCropoutGameInstance::GetInstance();
}
