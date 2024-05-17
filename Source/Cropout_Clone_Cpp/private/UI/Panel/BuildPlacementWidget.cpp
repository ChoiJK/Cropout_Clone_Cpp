// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/BuildPlacementWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Player/CropoutPlayer.h"
#include "Player/Components/PlacementHandler.h"
#include "UI/Element/ButtonWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CommonBorder.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/InGame/InGameGameMode.h"
#include "Data/BuildableInfo.h"

#include "SimpleRx/SimpleRx.h"

void UBuildPlacementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UCropoutGameInstance* GameInstance = UCropoutGameInstance::GetInstance();
	Player = Cast<ACropoutPlayer>(GameInstance->GetCurrentCropoutPlayerController()->GetPawn());

	PlaceButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnPlaceButtonClicked);
	RotateButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnRotateButtonClicked);
	CancelButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnCancelButtonClicked);

	CompositeDisposable = std::make_shared<SimpleRxCompositeDisposable>();
}

void UBuildPlacementWidget::NativeDestruct()
{
	Super::NativeDestruct();
	PlaceButton->OnClicked().RemoveAll(this);
	RotateButton->OnClicked().RemoveAll(this);
	CancelButton->OnClicked().RemoveAll(this);

	if(CompositeDisposable)
	{
		CompositeDisposable->Dispose();
		CompositeDisposable.reset();
	}
}

void UBuildPlacementWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(Player)
	{
		TObjectPtr<UPanelSlot> slot = RootBorder->Slot;
		TObjectPtr<UCanvasPanelSlot> canvasSlot = Cast<UCanvasPanelSlot>(slot);

		FVector2D screenPosition = Player->PlacementHandler->GetPlacementTargetBottomScreenPosition();
		float viewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

		canvasSlot->SetPosition(screenPosition / viewportScale);
	}
}

void UBuildPlacementWidget::OnPlaceButtonClicked()
{
	if(Player)
	{
		Player->PlacementHandler->PlacingEntity();
		SpendConstructionCost();
	}
}

void UBuildPlacementWidget::OnRotateButtonClicked()
{
	if(Player)
	{
		Player->PlacementHandler->RotatePlacementEntity();
	}
}

void UBuildPlacementWidget::OnCancelButtonClicked()
{
	if(Player)
	{
		Player->EndBuild();
	}

	DeactivateWidget();
}

void UBuildPlacementWidget::SetBuildableInfo(const FBuildableInfo* buildableInfo)
{
	BuildableInfo = buildableInfo;

	CompositeDisposable = std::make_shared<SimpleRxCompositeDisposable>();
	AInGameGameMode* gameMode = UCropoutGameInstance::GetInstance()->GetCurrentInGameMode();
	for(FConstructionCost cost : BuildableInfo->ConstructionCosts)
	{
		gameMode->GetResourceRP(cost.ResourceType)->SubscribeAndNotify([this](int value)
		{
			CheckHasConstructionCost();
		})->AddTo(CompositeDisposable);
	}
}

void UBuildPlacementWidget::CheckHasConstructionCost() const
{
	AInGameGameMode* gameMode = UCropoutGameInstance::GetInstance()->GetCurrentInGameMode();
	bool hasResourceCost = true;
	for(FConstructionCost cost : BuildableInfo->ConstructionCosts)
	{
		if(gameMode->HasResource(cost.ResourceType, cost.Coat) == false)
		{
			hasResourceCost = false;
			break;
		}
	}

	PlaceButton->SetIsInteractionEnabled(hasResourceCost);
}

void UBuildPlacementWidget::SpendConstructionCost() const
{
	AInGameGameMode* gameMode = UCropoutGameInstance::GetInstance()->GetCurrentInGameMode();
	for(FConstructionCost cost : BuildableInfo->ConstructionCosts)
	{
		gameMode->SpendResource(cost.ResourceType, cost.Coat);
	}
}
