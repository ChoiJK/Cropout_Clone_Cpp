// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/SplashLevel.h"

#include "Core/CropoutGameInstance.h"
#include "UI/UiManager.h"

void ASplashLevel::BeginPlay()
{
	Super::BeginPlay();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UUiManager* uiManager = gameInstance->GetLevelManager<UUiManager>();
	uiManager->CreateSplashWidget();
}

void ASplashLevel::Destroyed()
{
	Super::Destroyed();
}
