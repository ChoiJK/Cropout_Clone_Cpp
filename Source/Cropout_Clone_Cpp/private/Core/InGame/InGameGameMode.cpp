// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/InGame/InGameGameMode.h"

#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Player/CropoutPlayer.h"
#include "Enum/ResourceType.h"
#include "Manager/ResourceItemManager.h"

AInGameGameMode::AInGameGameMode()
{
	DefaultPawnClass = ACropoutPlayer::StaticClass();
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @ TODO : ScreenEffect FadeOut
}

void AInGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
