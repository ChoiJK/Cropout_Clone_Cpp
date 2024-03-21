// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

class UImage;
class UCommonTextBlock;
enum class EResourceType : uint8;

UCLASS()
class CROPOUT_CLONE_CPP_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EResourceType ResourceType;

	UPROPERTY(VisibleAnywhere)
	int ResourceAmount;

	UTexture2D* ResourceTexture[3];

	void SetAmountText() const;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ResourceImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* ResourceAmountText;

	virtual void NativeConstruct() override;

public:
	void SetValue(EResourceType type, int amount);
};
