// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/InGameLavel.h"

#include "Core/CropoutGameInstance.h"
#include "UI/UiManager.h"

void AInGameLavel::BeginPlay()
{
	Super::BeginPlay();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UUiManager* uiManager = gameInstance->GetLevelManager<UUiManager>();
	if(uiManager)
	{
		uiManager->CreateInGameLayerWidget();
	}
}

void AInGameLavel::Destroyed()
{
	Super::Destroyed();
}
