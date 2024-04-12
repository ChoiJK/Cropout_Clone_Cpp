// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/BuildEntityCardWidget.h"

#include "Components/Image.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Data/BuildableInfo.h"
#include "Player/CropoutPlayer.h"
#include "UI/UiManager.h"
#include "UI/Element/BuildEntityInfoWidget.h"

void UBuildEntityCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->OnClicked().AddUObject(this, &UBuildEntityCardWidget::OnButtonClicked);
}

void UBuildEntityCardWidget::SetBuildableInfo(const FBuildableInfo* buildableInfo)
{
	BuildableInfo = buildableInfo;
	BuildEntityInfoWidget->SetBuildableInfo(buildableInfo);

	UCropoutGameInstance* GameInstance = UCropoutGameInstance::GetInstance();
	GameInstance->GetStreamableManager().RequestAsyncLoad(buildableInfo->UIIcon.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, buildableInfo]()
		{
			this->BuildEntityImage->SetBrushFromTexture(
				buildableInfo->UIIcon.Get());
		}));
}

void UBuildEntityCardWidget::OnButtonClicked()
{
	if(BuildableInfo != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBuildEntityCardWidget::OnButtonClicked, %s"), *BuildableInfo->ToString());
		UCropoutGameInstance* GameInstance = UCropoutGameInstance::GetInstance();

		ACropoutPlayer* Player = Cast<ACropoutPlayer>(GameInstance->GetCurrentPlayerController()->GetPawn());
		if(Player)
		{
			Player->BeginBuild(BuildableInfo);
		}

		UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
		UTableManager* tableManager = gameInstance->GetSystemManager<UTableManager>();
		UUiManager* uiManager = gameInstance->GetCurrentLevelScript()->GetLevelManager<UUiManager>();

		TSubclassOf<UUserWidget>* buildPanel = tableManager->GetWidgetClass(EWidgetType::BuildPlacement);
		uiManager->GetUIBase()->PushBottomClass(buildPanel->Get());
	}
}
