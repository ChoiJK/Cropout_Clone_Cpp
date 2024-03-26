// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiBase.h"

#include "Blueprint/WidgetTree.h"
#include "Widgets/CommonactivatableWidgetContainer.h"
#include "Components/Overlay.h"

void UUiBase::NativeConstruct()
{
	Super::NativeConstruct();
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
	MenuStack = WidgetTree->ConstructWidget<UCommonActivatableWidgetStack>(
		UCommonActivatableWidgetStack::StaticClass(), TEXT("MenuStack"));
	PromptStack = WidgetTree->ConstructWidget<UCommonActivatableWidgetStack>(
		UCommonActivatableWidgetStack::StaticClass(), TEXT("PromptStack"));

	RootWidget->AddChild(Overlay);
	Overlay->AddChild(MenuStack);
	Overlay->AddChild(PromptStack);
}

void UUiBase::PushActivatableWidget(TSubclassOf<UCommonActivatableWidget> Menu)
{
	MenuStack->AddWidget(Menu);
}
