#pragma once

#include "Engine/DataTable.h"
#include "Enum/VillagerJobType.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"

#include "Enum/ResourceType.h"
#include "VillagerJob.generated.h"

USTRUCT(BlueprintType)
struct FVillagerJob : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> WorkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> Hat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Tool;
};

inline EVillagerJobType GetVillagerJobTypeFromString(const FString& jobTypeString)
{
	if(jobTypeString == "Idle")
	{
		return EVillagerJobType::Idle;
	}
	if(jobTypeString == "Food")
	{
		return EVillagerJobType::Food;
	}
	if(jobTypeString == "Wood")
	{
		return EVillagerJobType::Wood;
	}
	if(jobTypeString == "Stone")
	{
		return EVillagerJobType::Stone;
	}
	if(jobTypeString == "Farming")
	{
		return EVillagerJobType::Farming;
	}
	if(jobTypeString == "Build")
	{
		return EVillagerJobType::Build;
	}
	return EVillagerJobType::None;
}

inline FString GetVillagerJobTypeString(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::None:
		return "None";
	case EVillagerJobType::Idle:
		return "Idle";
	case EVillagerJobType::Food:
		return "Food";
	case EVillagerJobType::Wood:
		return "Wood";
	case EVillagerJobType::Stone:
		return "Stone";
	case EVillagerJobType::Farming:
		return "Farming";
	case EVillagerJobType::Build:
		return "Build";
	default:
		return "None";
	}
}

inline bool IsJobTypeResource(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::Food:
	case EVillagerJobType::Wood:
	case EVillagerJobType::Stone:
		return true;
	default:
		return false;
	}
}

inline bool IsJobTypeBuilding(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::Build:
		return true;
	default:
		return false;
	}
}

inline bool IsJobTypeFarming(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::Farming:
		return true;
	default:
		return false;
	}
}

inline EResourceType GetResourceTypeFromJobType(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::Food:
		return EResourceType::Food;
	case EVillagerJobType::Wood:
		return EResourceType::Wood;
	case EVillagerJobType::Stone:
		return EResourceType::Stone;
	case EVillagerJobType::Farming:
		return EResourceType::Food;
	default:
		return EResourceType::None;
	}
}
