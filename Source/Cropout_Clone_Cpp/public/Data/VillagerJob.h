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
	TSoftClassPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UAnimMontage> WorkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<USkeletalMesh> Hat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UStaticMesh> Tool;
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
