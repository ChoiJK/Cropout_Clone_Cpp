// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Panel/BuildPlacementWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Player/CropoutPlayer.h"
#include "Player/Components/PlacementHandler.h"
#include "UI/Element/ButtonWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CommonBorder.h"
#include "Components/CanvasPanelSlot.h"

void UBuildPlacementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UCropoutGameInstance* GameInstance = UCropoutGameInstance::GetInstance();
	Player = Cast<ACropoutPlayer>(GameInstance->GetCurrentPlayerController()->GetPawn());

	PlaceButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnPlaceButtonClicked);
	RotateButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnRotateButtonClicked);
	CancelButton->OnClicked().AddUObject(this, &UBuildPlacementWidget::OnCancelButtonClicked);
}

void UBuildPlacementWidget::NativeDestruct()
{
	Super::NativeDestruct();
	PlaceButton->OnClicked().RemoveAll(this);
	RotateButton->OnClicked().RemoveAll(this);
	CancelButton->OnClicked().RemoveAll(this);
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
