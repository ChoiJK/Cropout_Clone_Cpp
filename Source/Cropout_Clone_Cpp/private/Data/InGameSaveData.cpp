// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/InGameSaveData.h"

void UInGameSaveData::SetResourceBank(EResourceType type, int amount)
{
	ResourceBank[static_cast<int>(type)] = amount;
}

int UInGameSaveData::GetResourceBank(EResourceType type) const
{
	return ResourceBank[static_cast<int>(type)];
}

void UInGameSaveData::SetIslandRandSeed(int seed)
{
	IslandRandSeed = seed;
}

int UInGameSaveData::GetIslandRandSeed() const
{
	return IslandRandSeed;
}
