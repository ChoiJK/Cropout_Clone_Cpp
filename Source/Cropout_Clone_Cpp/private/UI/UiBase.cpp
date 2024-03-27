// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiBase.h"

#include "Widgets/CommonactivatableWidgetContainer.h"

void UUiBase::NativeConstruct()
{
	Super::NativeConstruct();
}

UCommonActivatableWidget* UUiBase::PushActivatableWidgetClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return MenuStack->AddWidget(widgetClass);
}
