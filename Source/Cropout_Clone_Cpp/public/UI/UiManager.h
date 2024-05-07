// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Base/LevelManager.h"
#include "Data/WidgetTableData.h"
#include "UI/UiBase.h"
#include "UiManager.generated.h"

class AInGameGameMode;
class UInGameLayerWidget;
class UResourceWidget;
class UTableManager;

DECLARE_MULTICAST_DELEGATE(FVoidDelegate);
UCLASS()
class CROPOUT_CLONE_CPP_API UUiManager : public ULevelManager
{
	GENERATED_BODY()
	UPROPERTY()
	UUiBase* UIBase;

	UPROPERTY()
	UCommonActivatableWidget* UI_Main;

	UPROPERTY()
	UInGameLayerWidget* UI_InGameMain;

	UPROPERTY()
	UTableManager* TableManager;

	bool IsInitialized = false;

	TSharedPtr<FVoidDelegate> OnInitializeSuccess;

protected:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	void InternalCreateSplashWidget();
	void InternalCreateMainMenuWidget();
	void InternalCreateInGameLayerWidget();

public:
	TSubclassOf<UUserWidget>* GetWidgetClass(EWidgetType type);

	UUiBase* GetUIBase() const;

	void CreateSplashWidget();
	void CreateMainMenuWidget();
	void CreateInGameLayerWidget();

	void UpdateVillagerCount();
};
