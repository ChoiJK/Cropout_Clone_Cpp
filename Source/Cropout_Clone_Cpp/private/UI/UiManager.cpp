// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiManager.h"

#include "Core/CropoutGameInstance.h"
#include "Core/InGame/CropoutGameMode.h"
#include "UI/Panel/InGameLayerWidget.h"
#include "UI/Element/ResourceWidget.h"
#include "Manager/TableManager.h"

// Called when the game starts or when spawned
void UUiManager::Init()
{
	Super::Init();

	OnInitializeSuccess = MakeShareable(new FVoidDelegate());

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
		IsInitialized = true;
		OnInitializeSuccess->Broadcast();
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
	OnInitializeSuccess->Clear();
}

UUiBase* UUiManager::GetUIBase() const
{
	return UIBase;
}

void UUiManager::InternalCreateSplashWidget()
{
	TSubclassOf<UUserWidget>* splashWidget = TableManager->GetWidgetClass(EWidgetType::Splash);
	if(splashWidget)
	{
		UI_Main = UIBase->PushMenuClass(splashWidget->Get());
	}
}

void UUiManager::InternalCreateMainMenuWidget()
{
	TSubclassOf<UUserWidget>* mainMenuWidget = TableManager->GetWidgetClass(EWidgetType::MainMenu);
	if(mainMenuWidget)
	{
		UI_Main = UIBase->PushMenuClass(mainMenuWidget->Get());
	}
}

void UUiManager::InternalCreateInGameLayerWidget()
{
	TSubclassOf<UUserWidget>* userWidget = TableManager->GetWidgetClass(EWidgetType::InGameMain);
	if(userWidget)
	{
		UI_InGameMain = Cast<UInGameLayerWidget>(UIBase->PushMenuClass(userWidget->Get()));
	}

	TSubclassOf<UUserWidget>* buildOpenButton = TableManager->GetWidgetClass(EWidgetType::BuildOpenButton);
	if(buildOpenButton)
	{
		UIBase->PushBottomClass(buildOpenButton->Get());
	}
}

void UUiManager::CreateSplashWidget()
{
	if(IsInitialized)
	{
		InternalCreateSplashWidget();
	}
	else
	{
		OnInitializeSuccess->AddLambda([this]()
		{
			InternalCreateSplashWidget();
		});
	}
}

void UUiManager::CreateMainMenuWidget()
{
	if(IsInitialized)
	{
		InternalCreateMainMenuWidget();
	}
	else
	{
		OnInitializeSuccess->AddLambda([this]()
		{
			InternalCreateMainMenuWidget();
		});
	}
}

void UUiManager::CreateInGameLayerWidget()
{
	if(IsInitialized)
	{
		InternalCreateInGameLayerWidget();
	}
	else
	{
		OnInitializeSuccess->AddLambda([this]()
		{
			InternalCreateInGameLayerWidget();
		});
	}
}

void UUiManager::UpdateVillagerCount()
{
	if(IsValid(UI_InGameMain))
	{
		UI_InGameMain->SetVillagerCount(GameInstance->GetCurrentGameMode()->GetVillagerCount());
	}
}
