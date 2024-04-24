// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BuildPlacementWidget.generated.h"

class UButtonWidget;
class ACropoutPlayer;
class UCommonBorder;
class SimpleRxCompositeDisposable;
struct FBuildableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildPlacementWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	std::shared_ptr<SimpleRxCompositeDisposable> CompositeDisposable;
	ACropoutPlayer* Player = nullptr;
	const FBuildableInfo* BuildableInfo = nullptr;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPlaceButtonClicked();
	UFUNCTION()
	void OnRotateButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

	void CheckHasConstructionCost() const;
	void SpendConstructionCost() const;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonBorder* RootBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* PlaceButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* RotateButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* CancelButton;

public:
	void SetBuildableInfo(const FBuildableInfo* buildableInfo);
};
