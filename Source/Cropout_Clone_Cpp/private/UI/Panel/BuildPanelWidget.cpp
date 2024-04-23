#include "UI/Panel/BuildPanelWidget.h"
#include "Core/CropoutGameInstance.h"
#include "Manager/TableManager.h"


#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "UI/Element/ButtonWidget.h"
#include "UI/Element/BuildEntityCardWidget.h"

void UBuildPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked().AddUObject(this, &UBuildPanelWidget::OnBackButtonClicked);

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	TableManager = gameInstance->GetSystemManager<UTableManager>();
}

void UBuildPanelWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	BuildItemBox->ClearChildren();

	TArray<const FBuildableInfo*> BuildableInfos = TableManager->GetBuildableInfos();
	bool isLastEntity = false;
	for(const FBuildableInfo* buildableInfo : BuildableInfos)
	{
		isLastEntity = buildableInfo == BuildableInfos.Last();

		TSubclassOf<UUserWidget>* buildEntityCardClass = TableManager->GetWidgetClass(EWidgetType::BuildEntityCard);

		UBuildEntityCardWidget* card = CreateWidget<UBuildEntityCardWidget>(this, buildEntityCardClass->Get());
		if(card)
		{
			card->SetBuildableInfo(buildableInfo);
			UHorizontalBoxSlot* slot = BuildItemBox->AddChildToHorizontalBox(card);
			slot->SetHorizontalAlignment(HAlign_Center);
			slot->SetVerticalAlignment(VAlign_Bottom);

			if(isLastEntity == false)
			{
				USpacer* spacer = NewObject<USpacer>(this);
				spacer->SetSize(FVector2D(20, 0));
				BuildItemBox->AddChild(spacer);
			}
		}
	}
}

void UBuildPanelWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	BuildItemBox->ClearChildren();
}

void UBuildPanelWidget::OnBackButtonClicked()
{
	DeactivateWidget();
}
