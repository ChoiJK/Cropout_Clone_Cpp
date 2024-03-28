// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/PauseMenuWidget.h"

#include "UI/Element/ButtonWidget.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked().AddUObject(this, &UPauseMenuWidget::OnResumeButtonClicked);
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
	DeactivateWidget();
}
