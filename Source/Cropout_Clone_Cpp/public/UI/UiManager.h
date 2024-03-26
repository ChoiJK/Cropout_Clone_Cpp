// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ResourceType.h"
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

public:
	// Sets default values for this actor's properties
	AUiManager();
	static AUiManager* Instance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameLayerWidget> InGameLayerWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UResourceWidget> ResourceWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	void UpdateResourcesWidget(EResourceType type);
	void UpdateVillagerCount();
};
