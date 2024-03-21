// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiManager.h"

#include "UI/InGameLayerWidget.h"
#include "UI/ResourceWidget.h"
AUiManager* AUiManager::Instance = nullptr;

// Sets default values
AUiManager::AUiManager()
{
	Instance = this;
}

// Called when the game starts or when spawned
void AUiManager::BeginPlay()
{
	Super::BeginPlay();

	check(IsValid(InGameLayerWidgetClass));
	check(IsValid(ResourceWidgetClass));
}

void AUiManager::Destroyed()
{
	Super::Destroyed();
	Instance = this;
}
