// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainMenuWidget.generated.h"

class UButtonWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UMainMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void OnNewGameButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* NewGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* QuitButton;
};
