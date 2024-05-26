// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_SetResultMenuWidgetText.h"

#include "UI/Panel/ResultMenuWidget.h"

void UStateProcess_SetResultMenuWidgetText::Init()
{
}

void UStateProcess_SetResultMenuWidgetText::Process(UStateProcessSharedData* sharedData)
{
	UCommonActivatableWidget* currentWidget = sharedData->GetCurrentActivatableWidget();
	if(currentWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentActivatableWidget is nullptr"));
		return;
	}

	const UResultMenuWidget* resultMenuWidget = Cast<UResultMenuWidget>(currentWidget);

	if(resultMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResultMenuWidget is not opened"));
	}

	resultMenuWidget->SetResultText(FText::FromString(ResultMenuWidgetText));
}

bool UStateProcess_SetResultMenuWidgetText::IsSuccess()
{
	return true;
}

void UStateProcess_SetResultMenuWidgetText::SetMenuText(FString menuString)
{
	ResultMenuWidgetText = menuString;
}
