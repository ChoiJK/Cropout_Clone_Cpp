// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ResourceType.h"
#include "Data/WidgetTableData.h"
#include "UiManager.generated.h"

class ACropoutGameMode;
class UUiBase;
class UInGameLayerWidget;
class UResourceWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API AUiManager : public AActor
{
	GENERATED_BODY()

	ACropoutGameMode* GameMode;

	UUiBase* UIBase;
	UInGameLayerWidget* UI_HUD;
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetTable;

public:
	// Sets default values for this actor's properties
	static AUiManager* Instance;
	AUiManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	TSubclassOf<UUserWidget>* GetWidgetClass(EWidgetType type);

	void UpdateResourcesWidget(EResourceType type);
	void UpdateVillagerCount();
};
