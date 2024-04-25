// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "BuildEntityCardWidget.generated.h"

class UBuildEntityInfoWidget;
class UImage;
class UTexture2D;
struct FBuildableInfo;
class SimpleRxCompositeDisposable;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildEntityCardWidget : public UCommonButtonBase
{
	GENERATED_BODY()

	const FBuildableInfo* BuildableInfo;

	void CheckHasConstructionCost();
	std::shared_ptr<SimpleRxCompositeDisposable> CompositeDisposable;

protected:
	void OnButtonClicked();
	void OnButtonHovered();
	void OnButtonUnhovered();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Hightlight_In;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Hightlight_Out;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Loop_Hover;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BuildEntityImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBuildEntityInfoWidget* BuildEntityInfoWidget;

public:
	void SetBuildableInfo(const FBuildableInfo* buildableInfo);
};
