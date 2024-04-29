// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/ResultMenuWidget.h"

#include "CommonTextBlock.h"
#include "Core/CropoutGameInstance.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "UI/Element/ButtonWidget.h"

void UResultMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UResultMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PlayAgainButton->OnClicked().AddUObject(this, &UResultMenuWidget::OnPlayAgainButtonClicked);
	ContinueButton->OnClicked().AddUObject(this, &UResultMenuWidget::OnContinueButtonClicked);
	ToMainMenuButton->OnClicked().AddUObject(this, &UResultMenuWidget::OnToMainMenuButtonClicked);
}

void UResultMenuWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	PlayAgainButton->OnClicked().RemoveAll(this);
	ContinueButton->OnClicked().RemoveAll(this);
	ToMainMenuButton->OnClicked().RemoveAll(this);
}

void UResultMenuWidget::SetResultText(FText string) const
{
	ResultText->SetText(string);
}

void UResultMenuWidget::OnPlayAgainButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::RestartGame);
}

void UResultMenuWidget::OnContinueButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::ResumeGame);
}

void UResultMenuWidget::OnToMainMenuButtonClicked()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->TriggerStateTransition(EStateTrigger::ToMainMenu);
}
