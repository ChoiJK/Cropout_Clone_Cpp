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

void UInGameSaveData::AddEntityData(FSpawnEntitySaveData data)
{
	SpawnEntityData.Add(data);
}

TArray<FSpawnEntitySaveData>& UInGameSaveData::GetEntityData()
{
	return SpawnEntityData;
}

void UInGameSaveData::AddDecorationTransform(FName name, FTransform transform)
{
	if(DecorationTransforms.Contains(name) == false)
	{
		FTransformsSaveData data;
		data.Transforms.Add(transform);
		DecorationTransforms.Add(name, data);
	}
	else
	{
		DecorationTransforms[name].Transforms.Add(transform);
	}
}

TMap<FName, FTransformsSaveData>& UInGameSaveData::GetDecorationTransforms()
{
	return DecorationTransforms;
}

void UInGameSaveData::AddVillagerData(FVillagerSaveData data)
{
	VillagerData.Add(data);
}

TArray<FVillagerSaveData>& UInGameSaveData::GetVillagerData()
{
	return VillagerData;
}
