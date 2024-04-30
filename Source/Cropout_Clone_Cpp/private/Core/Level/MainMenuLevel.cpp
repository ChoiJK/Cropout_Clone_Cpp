// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/MainMenuLevel.h"

#include "Core/CropoutGameInstance.h"
#include "UI/UiManager.h"


void AMainMenuLevel::BeginPlay()
{
	Super::BeginPlay();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UUiManager* uiManager = gameInstance->GetLevelManager<UUiManager>();
	uiManager->CreateMainMenuWidget();

	if(APlayerController* playerController = gameInstance->GetCurrentPlayerController())
	{
		playerController->SetInputMode(FInputModeUIOnly());
		playerController->bShowMouseCursor = true;
	}
}

void AMainMenuLevel::Destroyed()
{
	Super::Destroyed();
}
