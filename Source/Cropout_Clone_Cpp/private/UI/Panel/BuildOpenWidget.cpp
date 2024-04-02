// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/BuildOpenWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"
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
	UCropoutGameInstance* gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	UTableManager* tableManager = gameInstance->GetSystemManager<UTableManager>();
	UUiManager* uiManager = gameInstance->GetCurrentLevelScript()->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* buildPanel = tableManager->GetWidgetClass(EWidgetType::BuildPanel);
	uiManager->GetUIBase()->PushBottomClass(buildPanel->Get());
}
