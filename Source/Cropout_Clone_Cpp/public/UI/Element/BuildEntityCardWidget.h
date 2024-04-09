// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Enum/BuildEntityType.h"
#include "BuildEntityCardWidget.generated.h"

class UBuildEntityInfoWidget;
class UImage;
class UTexture2D;
struct FBuildableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API UBuildEntityCardWidget : public UCommonButtonBase
{
	GENERATED_BODY()

	const FBuildableInfo* BuildableInfo;

	UPROPERTY(EditAnywhere)
	EBuildEntityType BuildEntityType;

protected:
	void OnButtonClicked();
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* BuildEntityImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBuildEntityInfoWidget* BuildEntityInfoWidget;

public:
	void SetBuildableInfo(const FBuildableInfo* buildableInfo);
};
