// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/MainMenuWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "UI/Element/ButtonWidget.h"

void UMainMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	ContinueButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnContinueButtonClicked);
	NewGameButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnNewGameButtonClicked);
	QuitButton->OnClicked().AddUObject(this, &UMainMenuWidget::OnQuitButtonClicked);
}

void UMainMenuWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ContinueButton->OnClicked().Clear();
	NewGameButton->OnClicked().Clear();
	QuitButton->OnClicked().Clear();
}

void UMainMenuWidget::OnContinueButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::LoadGame);
}

void UMainMenuWidget::OnNewGameButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::NewGame);
}

void UMainMenuWidget::OnQuitButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::ExitGame);
}
