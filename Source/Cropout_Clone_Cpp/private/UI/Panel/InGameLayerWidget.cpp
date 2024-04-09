// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/InGameLayerWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/Element/ResourceWidget.h"
#include "Enum/ResourceType.h"
#include "UI/Element/ButtonWidget.h"
#include "UI/UiManager.h"
#include "CommonButtonBase.h"

void UInGameLayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ResourceContainer);

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	TableManager = gameInstance->GetSystemManager<UTableManager>();
	UiManager = gameInstance->GetCurrentLevelScript()->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* resourceWidget = TableManager->GetWidgetClass(EWidgetType::ResourceElement);

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

	PauseButton->OnClicked().AddUObject(this, &UInGameLayerWidget::OnPauseButtonClicked);
}

void UInGameLayerWidget::OnPauseButtonClicked()
{
	if(TableManager == nullptr || UiManager == nullptr)
	{
		return;
	}

	TSubclassOf<UUserWidget>* pauseMenu = TableManager->GetWidgetClass(EWidgetType::PauseMenu);

	UiManager->GetUIBase()->PushPromptClass(pauseMenu->Get());
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
