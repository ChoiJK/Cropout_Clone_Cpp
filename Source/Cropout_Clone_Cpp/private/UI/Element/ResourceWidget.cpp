// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/ResourceWidget.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Enum/ResourceType.h"

void UResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResourceTexture[0] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Food_DA.T_Rsrc_Food_DA'"));
	ResourceTexture[1] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Wood_DA.T_Rsrc_Wood_DA'"));
	ResourceTexture[2] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Stone_DA.T_Rsrc_Stone_DA'"));

	check(ResourceImage);
	check(ResourceAmountText);

	check(ResourceTexture[0]);
	check(ResourceTexture[1]);
	check(ResourceTexture[2]);

	SetImage();
	SetAmountText();
}

void UResourceWidget::SetAmountText() const
{
	if(IsValid(ResourceAmountText))
	{
		ResourceAmountText->SetText(FText::FromString(FString::FromInt(ResourceAmount)));
	}
}

void UResourceWidget::SetImage() const
{
	if(IsValid(ResourceImage))
	{
		ResourceImage->SetBrushFromTexture(ResourceTexture[static_cast<int>(ResourceType) - 1]);
	}
}

void UResourceWidget::SetResourceType(EResourceType type)
{
	if(ResourceType != type)
	{
		ResourceType = type;
		SetImage();
	}
}

void UResourceWidget::SetValue(int amount)
{
	if(ResourceAmount != amount)
	{
		ResourceAmount = amount;
		SetAmountText();
	}
}
