// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Enum/BuildEntityType.h"
#include "BuildEntityCardWidget.generated.h"

class UBuildEntityInfoWidget;
class UImage;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildEntityCardWidget : public UCommonButtonBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EBuildEntityType BuildEntityType;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BuildEntityImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBuildEntityInfoWidget* BuildEntityInfoWidget;
};
