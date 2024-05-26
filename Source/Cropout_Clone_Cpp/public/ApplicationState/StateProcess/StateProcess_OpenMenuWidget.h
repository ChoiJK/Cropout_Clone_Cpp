// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateProcessBase.h"
#include "Enum/WidgetType.h"
#include "StateProcess_OpenMenuWidget.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcess_OpenMenuWidget : public UStateProcessBase
{
	GENERATED_BODY()

	bool isOpened = false;
	EWidgetType WidgetType;

public:
	virtual void Init() override;
	virtual void Process(UStateProcessSharedData* sharedData) override;
	virtual bool IsSuccess() override;
	void SetWidgetType(EWidgetType Widget);
};
