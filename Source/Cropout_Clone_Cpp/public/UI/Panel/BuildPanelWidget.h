// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BuildPanelWidget.generated.h"

class UHorizontalBox;
class UButtonWidget;
class UTableManager;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildPanelWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UTableManager* TableManager;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* BuildItemBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWidget* BackButton;
};
