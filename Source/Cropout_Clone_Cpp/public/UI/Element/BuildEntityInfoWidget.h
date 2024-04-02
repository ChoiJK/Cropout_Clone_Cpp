// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/BuildEntityType.h"
#include "BuildEntityInfoWidget.generated.h"

class UImage;
class UHorizontalBox;
class UCommonTextBlock;
struct FBuildableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildEntityInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EBuildEntityType BuildEntityType;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCommonTextBlock* BuildEntityName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ResourceBox;

	virtual void NativeConstruct() override;

public:
	void SetBuildableInfo(const FBuildableInfo* buildableInfo);
};
