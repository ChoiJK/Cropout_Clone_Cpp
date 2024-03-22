// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameLayerWidget.h"
#include "Components/VerticalBox.h"
#include "UI/ResourceWidget.h"
#include "Interactable/Resource/ResourceBaseActor.h"
#include "UI/UiManager.h"

void UInGameLayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ResourceContainer);

	if(IsValid(AUiManager::Instance->ResourceWidgetClass))
	{
		for(int i = 0; i < 3; ++i)
		{
			ResourceWidget[i] = CreateWidget<UResourceWidget>(this, AUiManager::Instance->ResourceWidgetClass);
			ResourceContainer->AddChildToVerticalBox(ResourceWidget[i]);
			ResourceWidget[i]->SetResourceType(static_cast<EResourceType>(i + 1));
			ResourceWidget[i]->SetValue(0);
		}
	}
}

UResourceWidget* UInGameLayerWidget::GetResourceWidget(int index) const
{
	if(index >= 0 && index < 3)
	{
		return ResourceWidget[index];
	}

	return nullptr;
}
