// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ButtonWidget.generated.h"

class UCommonTextBlock;
class UCommonActionWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UButtonWidget : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCommonActionWidget* ActionWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCommonTextBlock* ButtonText;

	UPROPERTY(EditAnywhere)
	FText Text;

	virtual void NativePreConstruct() override;
};
