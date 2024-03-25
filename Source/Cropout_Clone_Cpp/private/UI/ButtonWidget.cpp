// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ButtonWidget.h"

#include "CommonTextBlock.h"
#include "CommonActionWidget.h"

void UButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(IsValid(ButtonText))
	{
		ButtonText->SetText(Text);
	}

	if(IsValid(ActionWidget))
	{
		FDataTableRowHandle handle;
		ActionWidget->SetInputAction(handle);
	}
}
