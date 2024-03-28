// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseMenuWidget.generated.h"

class UButtonWidget;
class USoundSliderWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UPauseMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* ResumeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* RestartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* MainMenuButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USoundSliderWidget* AudioMusicSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USoundSliderWidget* AudioSFXSlider;
};
