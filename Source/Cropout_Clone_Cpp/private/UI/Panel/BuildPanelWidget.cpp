// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/BuildPanelWidget.h"

#include "UI/Element/ButtonWidget.h"

void UBuildPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked().AddUObject(this, &UBuildPanelWidget::OnBackButtonClicked);
}

void UBuildPanelWidget::OnBackButtonClicked()
{
	DeactivateWidget();
}
