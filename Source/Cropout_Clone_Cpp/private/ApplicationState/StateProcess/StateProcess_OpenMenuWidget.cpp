// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_OpenMenuWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"
#include "UI/UiManager.h"

void UStateProcess_OpenMenuWidget::Init()
{
	isOpened = false;
}

void UStateProcess_OpenMenuWidget::Process(UStateProcessSharedData* sharedData)
{
	if(sharedData->GetCurrentActivatableWidget() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivatableWidget is already opened"));
		return;
	}

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UTableManager* TableManager = gameInstance->GetSystemManager<UTableManager>();
	UUiManager* UiManager = gameInstance->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* pauseMenu = TableManager->GetWidgetClass(EWidgetType::PauseMenu);
	UCommonActivatableWidget* widget = UiManager->GetUIBase()->PushPromptClass(pauseMenu->Get());

	if(widget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivatableWidget is nullptr"));
		return;
	}

	sharedData->SetCurrentActivatableWidget(widget);
	isOpened = true;
}

bool UStateProcess_OpenMenuWidget::IsSuccess()
{
	return isOpened;
}

void UStateProcess_OpenMenuWidget::SetWidgetType(EWidgetType Widget)
{
	WidgetType = Widget;
}
