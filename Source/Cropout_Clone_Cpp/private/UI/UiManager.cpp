// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiManager.h"

#include "Core/CropoutGameMode.h"
#include "UI/InGameLayerWidget.h"
#include "UI/ResourceWidget.h"
#include "UI/UiBase.h"
#include "Enum/ResourceType.h"

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

	GameMode = Cast<ACropoutGameMode>(GetWorld()->GetAuthGameMode());

	check(IsValid(InGameLayerWidgetClass));
	check(IsValid(ResourceWidgetClass));
	// Create Main HUD
	if(IsValid(Instance->InGameLayerWidgetClass))
	{
		UIBase = Cast<UUiBase>(
			CreateWidget(GetWorld(), Instance->InGameLayerWidgetClass));
		UIBase->AddToViewport();
		UIBase->SetVisibility(ESlateVisibility::Visible);

		UIBase->PushActivatableWidget(InGameLayerWidgetClass);
	}
}

void AUiManager::Destroyed()
{
	Super::Destroyed();
	Instance = this;
}

void AUiManager::UpdateResourcesWidget(EResourceType type)
{
	const int resourceIndex = static_cast<int>(type) - 1;
	if(IsValid(UI_HUD))
	{
		UResourceWidget* resourceWidget = UI_HUD->GetResourceWidget(resourceIndex);
		if(IsValid(resourceWidget))
		{
			resourceWidget->SetValue(GameMode->GetResourceAmount(type));
		}
	}
}

void AUiManager::UpdateVillagerCount()
{
	if(IsValid(UI_HUD))
	{
		UI_HUD->SetVillagerCount(GameMode->GetVillagerCount());
	}

	// @TODO : gameInstance를 통해 저장
}
