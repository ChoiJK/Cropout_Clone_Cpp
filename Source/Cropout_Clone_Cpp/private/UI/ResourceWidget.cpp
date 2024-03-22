// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResourceWidget.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Interactable/Resource/ResourceBaseActor.h"

void UResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ResourceImage);
	check(ResourceAmountText);

	ResourceType = EResourceType::None;
	ResourceAmount = 0;
	SetAmountText();

	ResourceTexture[0] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Food_DA.T_Rsrc_Food_DA'"));
	ResourceTexture[1] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Wood_DA.T_Rsrc_Wood_DA'"));
	ResourceTexture[2] = LoadObject<UTexture2D>(
		nullptr, TEXT("Texture2D'/Game/UI/Materials/Textures/T_Rsrc_Stone_DA.T_Rsrc_Stone_DA'"));

	check(ResourceTexture[0]);
	check(ResourceTexture[1]);
	check(ResourceTexture[2]);
}


void UResourceWidget::SetAmountText() const
{
	if(IsValid(ResourceAmountText))
	{
		ResourceAmountText->SetText(FText::FromString(FString::FromInt(ResourceAmount)));
	}
}


void UResourceWidget::SetResourceType(EResourceType type)
{
	if(ResourceType != type)
	{
		ResourceType = type;
		ResourceImage->SetBrushFromTexture(ResourceTexture[static_cast<int>(type) - 1]);
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
