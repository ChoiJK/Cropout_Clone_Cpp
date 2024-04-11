// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiManager.h"

#include "Core/CropoutGameInstance.h"
#include "Core/CropoutGameMode.h"
#include "UI/Panel/InGameLayerWidget.h"
#include "UI/Element/ResourceWidget.h"
#include "Enum/ResourceType.h"

// Called when the game starts or when spawned
void UUiManager::Init()
{
	Super::Init();

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	{
		SetManagerName("UiManager");

		TableManager = GameInstance->GetSystemManager<UTableManager>();

		TSubclassOf<UUserWidget>* uiBase = TableManager->GetWidgetClass(EWidgetType::UiBase);
		UIBase = (CreateWidget<UUiBase>(GetWorld(), uiBase->Get()));
		if(IsValid(UIBase))
		{
			UIBase->AddToViewport();
			UIBase->SetVisibility(ESlateVisibility::Visible);
		}

		TSubclassOf<UUserWidget>* userWidget = TableManager->GetWidgetClass(EWidgetType::InGameMain);
		if(userWidget)
		{
			UI_HUD = Cast<UInGameLayerWidget>(UIBase->PushMenuClass(userWidget->Get()));
		}

		TSubclassOf<UUserWidget>* buildOpenButton = TableManager->GetWidgetClass(EWidgetType::BuildOpenButton);
		if(buildOpenButton)
		{
			UIBase->PushBottomClass(buildOpenButton->Get());
		}
	}));
}

void UUiManager::Prepare()
{
	Super::Prepare();
}

void UUiManager::Release()
{
	Super::Release();
}

void UUiManager::Destroy()
{
	Super::Destroy();
}

UUiBase* UUiManager::GetUIBase() const
{
	return UIBase;
}

void UUiManager::UpdateResourcesWidget(EResourceType type)
{
	const int resourceIndex = static_cast<int>(type) - 1;
	if(IsValid(UI_HUD))
	{
		UResourceWidget* resourceWidget = UI_HUD->GetResourceWidget(resourceIndex);
		if(IsValid(resourceWidget))
		{
			resourceWidget->SetValue(GameInstance->GetCurrentGameMode()->GetResourceAmount(type));
		}
	}
}

void UUiManager::UpdateVillagerCount()
{
	if(IsValid(UI_HUD))
	{
		UI_HUD->SetVillagerCount(GameInstance->GetCurrentGameMode()->GetVillagerCount());
	}

	// @TODO : gameInstance를 통해 저장
}
