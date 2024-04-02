// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Base/LevelManager.h"
#include "Enum/ResourceType.h"
#include "Data/WidgetTableData.h"
#include "UI/UiBase.h"
#include "UiManager.generated.h"

class ACropoutGameMode;
class UInGameLayerWidget;
class UResourceWidget;
class UTableManager;

UCLASS()
class CROPOUT_CLONE_CPP_API UUiManager : public ULevelManager
{
	GENERATED_BODY()

	UUiBase* UIBase;
	UInGameLayerWidget* UI_HUD;
	UTableManager* TableManager;

protected:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

public:
	TSubclassOf<UUserWidget>* GetWidgetClass(EWidgetType type);

	UUiBase* GetUIBase() const;

	void UpdateResourcesWidget(EResourceType type);
	void UpdateVillagerCount();
};
