// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMenu/CropoutMainMenuGameMode.h"
#include "Core/MainMenu/MainMenuPawn.h"

ACropoutMainMenuGameMode::ACropoutMainMenuGameMode()
{
	DefaultPawnClass = AMainMenuPawn::StaticClass();
}

void ACropoutMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}
