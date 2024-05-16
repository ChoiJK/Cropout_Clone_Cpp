// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/SoundSliderWidget.h"

#include "CommonTextBlock.h"
#include "Components/Slider.h"
#include "Core/CropoutGameInstance.h"
#include "Manager/SoundManager.h"


void USoundSliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Descriptor->SetText(SoundClassTitle);
}

void USoundSliderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateSlider();
	Slider->OnValueChanged.AddDynamic(this, &USoundSliderWidget::OnValueChanged);
}

void USoundSliderWidget::NativeDestruct()
{
	Super::NativeDestruct();
	Slider->OnValueChanged.RemoveDynamic(this, &USoundSliderWidget::OnValueChanged);
}

void USoundSliderWidget::UpdateSlider()
{
	UCropoutGameInstance* gi = UCropoutGameInstance::GetInstance();
	if(IsValid(gi))
	{
		USoundManager* soundManager = gi->GetSystemManager<USoundManager>();
		if(IsValid(soundManager))
		{
			Slider->SetValue(soundManager->GetSoundMix(static_cast<int>(Index)));
		}
	}
}

void USoundSliderWidget::OnValueChanged(float Value)
{
	UCropoutGameInstance* gi = UCropoutGameInstance::GetInstance();
	if(IsValid(gi))
	{
		USoundManager* soundManager = gi->GetSystemManager<USoundManager>();
		if(IsValid(soundManager))
		{
			soundManager->SetSoundMix(static_cast<int>(Index), Value, InSoundClass, InSoundMixModifier);
		}
	}
}
