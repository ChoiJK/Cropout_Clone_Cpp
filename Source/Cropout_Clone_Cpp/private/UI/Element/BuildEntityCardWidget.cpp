// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/BuildEntityCardWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Core/CropoutGameMode.h"
#include "Data/BuildableInfo.h"
#include "Manager/TableManager.h"
#include "Player/CropoutPlayer.h"

#include "UI/UiManager.h"
#include "UI/Element/BuildEntityInfoWidget.h"
#include "UI/Panel/BuildPlacementWidget.h"

#include "SimpleRx/SimpleRx.h"

void UBuildEntityCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FVector2d minDesiredSize = GetMinimumDesiredSize();
	minDesiredSize.Y = 250.f;
	SetMinimumDesiredSize(minDesiredSize);

	this->OnClicked().AddUObject(this, &UBuildEntityCardWidget::OnButtonClicked);
	this->OnHovered().AddUObject(this, &UBuildEntityCardWidget::OnButtonHovered);
	this->OnUnhovered().AddUObject(this, &UBuildEntityCardWidget::OnButtonUnhovered);
}

void UBuildEntityCardWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(CompositeDisposable)
	{
		CompositeDisposable->Dispose();
		CompositeDisposable.reset();
	}
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

	CompositeDisposable = std::make_shared<SimpleRxCompositeDisposable>();
	ACropoutGameMode* gameMode = UCropoutGameInstance::GetInstance()->GetCurrentGameMode();
	for(FConstructionCost cost : BuildableInfo->ConstructionCosts)
	{
		gameMode->GetResourceRP(cost.ResourceType)->SubscribeAndNotify([this](int value)
		{
			CheckHasConstructionCost();
		})->AddTo(CompositeDisposable);
	}
}

void UBuildEntityCardWidget::CheckHasConstructionCost()
{
	ACropoutGameMode* gameMode = UCropoutGameInstance::GetInstance()->GetCurrentGameMode();
	bool hasResourceCost = true;
	for(FConstructionCost cost : BuildableInfo->ConstructionCosts)
	{
		if(gameMode->HasResource(cost.ResourceType, cost.Coat) == false)
		{
			hasResourceCost = false;
			break;
		}
	}

	SetIsInteractionEnabled(hasResourceCost);
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
		UUiManager* uiManager = gameInstance->GetLevelManager<UUiManager>();

		TSubclassOf<UUserWidget>* buildPanel = tableManager->GetWidgetClass(EWidgetType::BuildPlacement);
		UBuildPlacementWidget* placementWidget = Cast<UBuildPlacementWidget>(
			uiManager->GetUIBase()->PushBottomClass(buildPanel->Get()));
		placementWidget->SetBuildableInfo(BuildableInfo);
	}
}

void UBuildEntityCardWidget::OnButtonHovered()
{
	FVector2d minDesiredSize = GetMinimumDesiredSize();
	minDesiredSize.Y = 300.f;
	SetMinimumDesiredSize(minDesiredSize);

	PlayAnimation(Loop_Hover, 0.f, 0);
	PlayAnimation(Hightlight_In, 0.f, 1);
}

void UBuildEntityCardWidget::OnButtonUnhovered()
{
	FVector2d minDesiredSize = GetMinimumDesiredSize();
	minDesiredSize.Y = 250.f;
	SetMinimumDesiredSize(minDesiredSize);

	StopAnimation(Loop_Hover);
	PlayAnimation(Hightlight_Out, 0.f, 1);
}
