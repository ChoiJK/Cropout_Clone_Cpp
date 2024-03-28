// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SoundSliderWidget.h"

#include "CommonTextBlock.h"
#include "Components/Slider.h"
#include "Core/CropoutGameInstance.h"
#include "Kismet/GameplayStatics.h"


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

void USoundSliderWidget::UpdateSlider()
{
	UCropoutGameInstance* gi = Cast<UCropoutGameInstance>(GetGameInstance());
	if(IsValid(gi))
	{
		USaveData* saveData = gi->GetSaveData();
		if(IsValid(saveData))
		{
			UE_LOG(LogTemp, Warning, TEXT("SoundMix: %f"), saveData->GetSoundMix(static_cast<int>(Index)));
			Slider->SetValue(saveData->GetSoundMix(static_cast<int>(Index)));
		}
	}
}

void USoundSliderWidget::OnValueChanged(float Value)
{
	UCropoutGameInstance* gi = Cast<UCropoutGameInstance>(GetGameInstance());
	if(IsValid(gi))
	{
		USaveData* saveData = gi->GetSaveData();
		if(IsValid(saveData))
		{
			UE_LOG(LogTemp, Warning, TEXT("SoundMix: %f"), Value);
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), InSoundMixModifier, InSoundClass, Value);
			saveData->SetSoundMix(static_cast<int>(Index), Value);
		}
	}
}
