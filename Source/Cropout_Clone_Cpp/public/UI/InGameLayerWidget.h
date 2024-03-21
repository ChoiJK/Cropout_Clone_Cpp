// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameLayerWidget.generated.h"

class UVerticalBox;
class UResourceWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UInGameLayerWidget : public UUserWidget
{
	GENERATED_BODY()

	UResourceWidget* ResourceWidget[3];

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ResourceContainer;

	virtual void NativeConstruct() override;
};
