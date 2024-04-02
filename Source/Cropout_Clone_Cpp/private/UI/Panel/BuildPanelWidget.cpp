// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"
#include "UI/Panel/BuildPanelWidget.h"

#include "Components/HorizontalBox.h"
#include "UI/Element/ButtonWidget.h"
#include "UI/Element/BuildEntityCardWidget.h"

void UBuildPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked().AddUObject(this, &UBuildPanelWidget::OnBackButtonClicked);

	UCropoutGameInstance* gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	UTableManager* tableManager = gameInstance->GetSystemManager<UTableManager>();
	TArray<const FBuildableInfo*> buildableInfos = tableManager->GetBuildableInfos();
	for(const FBuildableInfo* buildableInfo : buildableInfos)
	{
		TSubclassOf<UUserWidget>* buildEntityCardClass = tableManager->GetWidgetClass(EWidgetType::BuildEntityCard);

		UBuildEntityCardWidget* card = Cast<UBuildEntityCardWidget>(CreateWidget(this, buildEntityCardClass->Get()));
		if(card)
		{
			card->SetBuildableInfo(buildableInfo);
			BuildItemBox->AddChild(card);
		}
	}
}

void UBuildPanelWidget::OnBackButtonClicked()
{
	DeactivateWidget();
}
