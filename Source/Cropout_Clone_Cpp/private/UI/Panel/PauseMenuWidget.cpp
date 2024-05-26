// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/PauseMenuWidget.h"

#include "Core/CropoutGameInstance.h"
#include "ApplicationState/ApplicationStateManager.h"
#include "UI/Element/ButtonWidget.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPauseMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	ResumeButton->OnClicked().AddUObject(this, &UPauseMenuWidget::OnResumeButtonClicked);
	RestartButton->OnClicked().AddUObject(this, &UPauseMenuWidget::OnRestartButtonClicked);
	MainMenuButton->OnClicked().AddUObject(this, &UPauseMenuWidget::OnMainMenuButtonClicked);
}

void UPauseMenuWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ResumeButton->OnClicked().RemoveAll(this);
	RestartButton->OnClicked().RemoveAll(this);
	MainMenuButton->OnClicked().RemoveAll(this);
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::ResumeGame);
}

void UPauseMenuWidget::OnRestartButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::RestartGame);
}

void UPauseMenuWidget::OnMainMenuButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::ToMainMenu);
}
