// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/SystemManager.h"
#include "UObject/Object.h"
#include "Enum/WidgetType.h"
#include "Data/BuildableInfo.h"
#include "TableManager.generated.h"

struct FSpawnerInfo;
struct FInteractableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API UTableManager : public USystemManager
{
	GENERATED_BODY()

private:
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetTable;
	void InitializeWidgetTable();

	TMap<FName, FBuildableInfo*> BuildableTable;
	void InitializeBuildableTable();

	TMap<FName, FInteractableInfo*> InteractableTable;
	void InitializeInteractableTable();

	TMap<FName, FSpawnerInfo*> SpawnerTable;
	void InitializeSpawnerTable();

public:
	UTableManager();
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	// widget
	TSubclassOf<UUserWidget>* GetWidgetClass(EWidgetType type);

	// Buildable
	TArray<const FBuildableInfo*> GetBuildableInfos();

	// Interactable
	TArray<const FInteractableInfo*> GetInteractableInfos();
	const FInteractableInfo* GetInteractableInfo(FName name);

	// Spawner
	TArray<const FSpawnerInfo*> GetSpawnerInfo();
};
