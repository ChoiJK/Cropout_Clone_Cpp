// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ResourceType.h"
#include "GameFramework/SaveGame.h"
#include "InGameSaveData.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UInGameSaveData : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Options")
	int32 ResourceBank[static_cast<int>(EResourceType::Count)] = {0};

	UPROPERTY(VisibleAnywhere, Category = "Options")
	int32 IslandRandSeed = 0;

public:
	void SetResourceBank(EResourceType type, int amount);
	int GetResourceBank(EResourceType type) const;

	void SetIslandRandSeed(int seed);
	int GetIslandRandSeed() const;
};
