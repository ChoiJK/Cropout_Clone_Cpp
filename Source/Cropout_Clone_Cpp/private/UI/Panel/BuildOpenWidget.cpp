// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/BuildOpenWidget.h"
#include "UI/UiBase.h"
#include "UI/UiManager.h"
#include "UI/Element/ButtonWidget.h"

void UBuildOpenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BuildOpenButton->OnClicked().AddUObject(this, &UBuildOpenWidget::OnOpenButtonClicked);
}

void UBuildOpenWidget::OnOpenButtonClicked()
{
	TSubclassOf<UUserWidget>* buildPanel = AUiManager::Instance->GetWidgetClass(EWidgetType::BuildPanel);

	AUiManager::Instance->GetUIBase()->PushPromptClass(buildPanel->Get());
}
