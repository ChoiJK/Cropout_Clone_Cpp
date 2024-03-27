// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameLayerWidget.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/ResourceWidget.h"
#include "Enum/ResourceType.h"
#include "UI/ButtonWidget.h"
#include "UI/UiManager.h"

void UInGameLayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ResourceContainer);

	TSubclassOf<UUserWidget>* resourceWidget = AUiManager::Instance->GetWidgetClass(EWidgetType::ResourceElement);

	if(resourceWidget)
	{
		for(int i = 0; i < 3; ++i)
		{
			ResourceWidget[i] = CreateWidget<UResourceWidget>(this, resourceWidget->Get());
			ResourceContainer->AddChildToVerticalBox(ResourceWidget[i]);
			ResourceWidget[i]->SetResourceType(static_cast<EResourceType>(i + 1));
			ResourceWidget[i]->SetValue(0);
		}
	}

	// @Todo : GamePad상태일 때 PauseButton을 hide한다.
	//if(IsValid(PauseButton))
	//{
	//	UGameplayStatics::GetPlayerController(GetWorld(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ), 0)->bind
	//}
}

TOptional<FUIInputConfig> UInGameLayerWidget::GetDesiredInputConfig() const
{
	FUIInputConfig config = FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);

	return config;
}

UResourceWidget* UInGameLayerWidget::GetResourceWidget(int index) const
{
	if(index >= 0 && index < 3)
	{
		return ResourceWidget[index];
	}

	return nullptr;
}

void UInGameLayerWidget::SetVillagerCount(int count) const
{
	if(IsValid(VillagerCountText))
	{
		VillagerCountText->SetText(FText::FromString(FString::FromInt(count)));
	}
}
