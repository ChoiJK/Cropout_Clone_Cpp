// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BuildPlacementWidget.generated.h"

class UButtonWidget;
class ACropoutPlayer;
class UCommonBorder;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildPlacementWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	ACropoutPlayer* Player = nullptr;

	UFUNCTION()
	void OnPlaceButtonClicked();
	UFUNCTION()
	void OnRotateButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonBorder* RootBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* PlaceButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* RotateButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* CancelButton;
};
