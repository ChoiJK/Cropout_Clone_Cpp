// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "InGameLayerWidget.generated.h"

class UButtonWidget;
class UVerticalBox;
class UResourceWidget;
class UCommonTextBlock;

UCLASS()
class CROPOUT_CLONE_CPP_API UInGameLayerWidget : public UCommonActivatableWidget
{
public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

private:
	GENERATED_BODY()

	UResourceWidget* ResourceWidget[3];

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ResourceContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* VillagerCountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* PauseButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPauseButtonClicked();

public:
	UResourceWidget* GetResourceWidget(int index) const;
	void SetVillagerCount(int count) const;
};
