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

private:
	bool OnlyUIInput = false;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonActivatableWidgetStack* MainStack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonActivatableWidgetStack* PromptStack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonActivatableWidgetStack* BottomStack;

	UFUNCTION()
	void OnPromptStackChanged(UCommonActivatableWidget* CommonActivatableWidget);
	virtual void NativeConstruct() override;

public:
	UCommonActivatableWidget* PushMenuClass(TSubclassOf<UCommonActivatableWidget> widgetClass);
	UCommonActivatableWidget* PushPromptClass(TSubclassOf<UCommonActivatableWidget> widgetClass);
	UCommonActivatableWidget* PushBottomClass(TSubclassOf<UCommonActivatableWidget> widgetClass);
};
