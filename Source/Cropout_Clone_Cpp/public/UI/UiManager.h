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

UCLASS()
class CROPOUT_CLONE_CPP_API UUiManager : public ULevelManager
{
	GENERATED_BODY()

	ACropoutGameMode* GameMode;

	UUiBase* UIBase;
	UInGameLayerWidget* UI_HUD;
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetTable;

public:
	// Sets default values for this actor's properties
	static UUiManager* Instance;
	UUiManager();

protected:
	virtual void Initialize() override;
	virtual void Release() override;

public:
	TSubclassOf<UUserWidget>* GetWidgetClass(EWidgetType type);

	UUiBase* GetUIBase() const;

	void UpdateResourcesWidget(EResourceType type);
	void UpdateVillagerCount();
};
