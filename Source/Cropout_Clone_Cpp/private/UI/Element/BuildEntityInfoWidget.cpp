// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/BuildEntityInfoWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"
#include "UI/UiManager.h"

#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Data/BuildableInfo.h"
#include "Enum/WidgetType.h"
#include "UI/Element/ResourceWidget.h"

void UBuildEntityInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildEntityInfoWidget::SetBuildableInfo(const FBuildableInfo* buildableInfo)
{
	BackgroundImage->SetColorAndOpacity(buildableInfo->InfoBackgroundColor);
	BuildEntityName->SetText(FText::FromName(buildableInfo->Name));
	ResourceBox->ClearChildren();

	UCropoutGameInstance* gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	UTableManager* tableManager = gameInstance->GetSystemManager<UTableManager>();
	UUiManager* uiManager = gameInstance->GetCurrentLevelScript()->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* constructionResourcePanel = tableManager->GetWidgetClass(
		EWidgetType::ConstructionResource);
	for(const FConstructionCost constructionCost : buildableInfo->ConstructionCosts)
	{
		UResourceWidget* resourceWidget = CreateWidget<UResourceWidget>(this, constructionResourcePanel->Get());
		resourceWidget->SetResourceType(constructionCost.ResourceType);
		resourceWidget->SetValue(constructionCost.Coat);
		UHorizontalBoxSlot* slot = ResourceBox->AddChildToHorizontalBox(resourceWidget);
		slot->SetHorizontalAlignment(HAlign_Fill);
		slot->SetVerticalAlignment(VAlign_Fill);
		slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}
