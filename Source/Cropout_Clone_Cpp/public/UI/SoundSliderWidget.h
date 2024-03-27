// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/SoundMixType.h"
#include "SoundSliderWidget.generated.h"

class USlider;
class UCommonTextBlock;
class USoundClass;
class FText;
class USoundMix;

UCLASS()
class CROPOUT_CLONE_CPP_API USoundSliderWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* Descriptor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* Slider;

	UPROPERTY(EditAnywhere)
	// @Todo - SoundClass, SoundMixModifier는 추후 Table로 뺄 수 있음. [ESoundMixType, USoundClass, USoundMix, ClassTitle]
	USoundClass* InSoundClass;

	UPROPERTY(EditAnywhere)
	FText SoundClassTitle;

	UPROPERTY(EditAnywhere)
	USoundMix* InSoundMixModifier;

	UPROPERTY(EditAnywhere)
	ESoundMixType Index;

	UFUNCTION()
	void OnValueChanged(float Value);

public:
	void UpdateSlider();
};
