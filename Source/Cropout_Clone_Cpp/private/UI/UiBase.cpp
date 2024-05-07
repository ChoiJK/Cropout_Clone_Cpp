// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiBase.h"

#include "Widgets/CommonactivatableWidgetContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/InGame/InGamePlayerController.h"


void UUiBase::NativeConstruct()
{
	Super::NativeConstruct();
}

UCommonActivatableWidget* UUiBase::PushMenuClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return MainStack->AddWidget(widgetClass);
}

UCommonActivatableWidget* UUiBase::PushPromptClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return PromptStack->AddWidget(widgetClass);
}

UCommonActivatableWidget* UUiBase::PushBottomClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return BottomStack->AddWidget(widgetClass);
}
