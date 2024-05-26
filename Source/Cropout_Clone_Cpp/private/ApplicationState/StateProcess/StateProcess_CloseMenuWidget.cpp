// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_CloseMenuWidget.h"

#include "CommonActivatableWidget.h"

void UStateProcess_CloseMenuWidget::Init()
{
}

void UStateProcess_CloseMenuWidget::Process(UStateProcessSharedData* sharedData)
{
	UCommonActivatableWidget* ActivatableWidget = sharedData->GetCurrentActivatableWidget();

	if(ActivatableWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ctivatableWidget is already closed"));
		return;
	}

	if(ActivatableWidget != nullptr)
	{
		ActivatableWidget->DeactivateWidget();
		ActivatableWidget = nullptr;
		sharedData->SetCurrentActivatableWidget(nullptr);
	}
}

bool UStateProcess_CloseMenuWidget::IsSuccess()
{
	return true;
}
