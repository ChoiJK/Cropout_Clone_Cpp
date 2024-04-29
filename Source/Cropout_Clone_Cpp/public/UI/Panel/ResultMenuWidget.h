// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ResultMenuWidget.generated.h"

class UButtonWidget;
class UCommonTextBlock;

UCLASS()
class CROPOUT_CLONE_CPP_API UResultMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	UFUNCTION()
	void OnPlayAgainButtonClicked();

	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void OnToMainMenuButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* ResultText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* PlayAgainButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* ToMainMenuButton;

public:
	void SetResultText(FText string) const;
};
