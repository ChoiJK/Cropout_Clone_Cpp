// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiManager.h"

#include "Core/CropoutGameMode.h"
#include "Data/WidgetTableData.h"
#include "UI/Panel/InGameLayerWidget.h"
#include "UI/Element/ResourceWidget.h"
#include "Enum/ResourceType.h"

AUiManager* AUiManager::Instance = nullptr;

// Sets default values
AUiManager::AUiManager()
{
	Instance = this;

	ConstructorHelpers::FObjectFinder<UDataTable> villagerJob
		(TEXT("Engine.DataTable'/Game/UI/Table/WidgetDataTable.WidgetDataTable'"));
	check(villagerJob.Succeeded());

	if(villagerJob.Succeeded())
	{
		TArray<FName> rowNames = villagerJob.Object->GetRowNames();
		for(auto rowName : rowNames)
		{
			FWidgetTableData* widgetData = villagerJob.Object->FindRow<FWidgetTableData>(
				rowName, TEXT("WidgetDataTable"));
			if(widgetData && widgetData->WidgetType != EWidgetType::None)
			{
				WidgetTable.Add(widgetData->WidgetType, widgetData->WidgetClass);
			}
		}
	}
}

// Called when the game starts or when spawned
void AUiManager::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ACropoutGameMode>(GetWorld()->GetAuthGameMode());

	//TSubclassOf<UUserWidget>* ingameMain = WidgetTable.Find(EWidgetType::InGameMain);
	//UI_HUD = Cast<UInGameLayerWidget>(CreateWidget(GetWorld(), ingameMain->Get()));
	//if(IsValid(UI_HUD))
	//{
	//	UI_HUD->AddToViewport();
	//	UI_HUD->SetVisibility(ESlateVisibility::Visible);
	//}

	TSubclassOf<UUserWidget>* uiBase = WidgetTable.Find(EWidgetType::UiBase);
	UIBase = Cast<UUiBase>(CreateWidget(GetWorld(), uiBase->Get()));
	if(IsValid(UIBase))
	{
		UIBase->AddToViewport();
		UIBase->SetVisibility(ESlateVisibility::Visible);
	}

	TSubclassOf<UUserWidget>* userWidget = WidgetTable.Find(EWidgetType::InGameMain);
	if(userWidget)
	{
		UI_HUD = Cast<UInGameLayerWidget>(UIBase->PushMenuClass(userWidget->Get()));
	}
}

void AUiManager::Destroyed()
{
	Super::Destroyed();
	Instance = this;
}

TSubclassOf<UUserWidget>* AUiManager::GetWidgetClass(EWidgetType type)
{
	return WidgetTable.Find(type);
}

UUiBase* AUiManager::GetUIBase() const
{
	return UIBase;
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
