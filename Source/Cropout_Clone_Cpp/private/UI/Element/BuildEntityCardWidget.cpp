// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/BuildEntityCardWidget.h"

#include "Components/Image.h"
#include "Core/CropoutGameInstance.h"
#include "Data/BuildableInfo.h"
#include "UI/Element/BuildEntityInfoWidget.h"

void UBuildEntityCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildEntityCardWidget::SetBuildableInfo(const FBuildableInfo* buildableInfo)
{
	BuildableInfo = buildableInfo;
	BuildEntityInfoWidget->SetBuildableInfo(buildableInfo);

	UCropoutGameInstance* GameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	GameInstance->GetStreamableManager().RequestAsyncLoad(buildableInfo->UIIcon.ToSoftObjectPath(),
	                                                      FStreamableDelegate::CreateLambda([this, buildableInfo]()
	                                                      {
		                                                      this->BuildEntityImage->SetBrushFromTexture(
			                                                      buildableInfo->UIIcon.Get());
	                                                      }));
}
