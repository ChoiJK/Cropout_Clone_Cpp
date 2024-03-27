// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UiBase.generated.h"

class UOverlay;
class UCommonActivatableWidget;
class UCommonActivatableWidgetStack;

UCLASS()
class CROPOUT_CLONE_CPP_API UUiBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonActivatableWidgetStack* MenuStack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonActivatableWidgetStack* PromptStack;

	virtual void NativeConstruct() override;

public:
	UCommonActivatableWidget* PushActivatableWidgetClass(TSubclassOf<UCommonActivatableWidget> widgetClass);
};
