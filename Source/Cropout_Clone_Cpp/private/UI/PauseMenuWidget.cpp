// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"

#include "UI/ButtonWidget.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked().AddUObject(this, &UPauseMenuWidget::OnResumeButtonClicked);
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
	DeactivateWidget();
}
