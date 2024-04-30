// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/Image.h"
#include "SplashWidget.generated.h"

class UUMGSequencePlayer;

UCLASS()
class CROPOUT_CLONE_CPP_API USplashWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void OnSplashAlphaFadeOutFinished(UUMGSequencePlayer& sequencePlayer);

	UPROPERTY()
	UUMGSequencePlayer* SequencePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SplashAlphaFadeOut;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SplashImage;
};
