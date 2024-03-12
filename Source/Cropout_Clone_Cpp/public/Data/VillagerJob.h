#pragma once

#include "Engine/DataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"

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

enum class EVillagerJobType
{
	None,
	Idle
};

inline FString GetVillagerJobTypeString(EVillagerJobType jobType)
{
	switch(jobType)
	{
	case EVillagerJobType::None:
		return "None";
	case EVillagerJobType::Idle:
		return "Idle";
	default:
		return "None";
	}
}
