// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BuildOpenWidget.generated.h"

class UButtonWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildOpenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	UFUNCTION()
	void OnOpenButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* BuildOpenButton;
};
