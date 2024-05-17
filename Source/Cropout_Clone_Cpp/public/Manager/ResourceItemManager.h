// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LevelManager.h"
#include "Enum/ResourceType.h"
#include "SimpleRx/SimpleRxReactiveProperty.h"
#include "ResourceItemManager.generated.h"

class UInGameSaveData;

UCLASS()
class CROPOUT_CLONE_CPP_API UResourceItemManager : public ULevelManager
{
	GENERATED_BODY()
	TMap<EResourceType, std::shared_ptr<SimpleRxReactiveProperty<int>>> ResourceBank;
	void ResourceDebugMessage();

public:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	virtual void OnSaveData(UInGameSaveData* SaveData) override;
	virtual void OnLoadData(UInGameSaveData* SaveData) override;

	int GetResourceAmount(EResourceType type);
	bool HasResource(EResourceType type, int amount);
	void StoreResource(EResourceType type, int amount);
	void SpendResource(EResourceType type, int amount);
	bool ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount);

	std::shared_ptr<SimpleRxReactiveProperty<int>> GetResourceRP(EResourceType type);
};
