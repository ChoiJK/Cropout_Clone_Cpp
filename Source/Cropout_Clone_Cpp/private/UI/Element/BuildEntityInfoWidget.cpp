// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/BuildEntityInfoWidget.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Data/BuildableInfo.h"

void UBuildEntityInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildEntityInfoWidget::SetBuildableInfo(const FBuildableInfo* buildableInfo)
{
	BackgroundImage->SetColorAndOpacity(buildableInfo->InfoBackgroundColor);
	//BuildEntityName->(buildableInfo->Name);
}
