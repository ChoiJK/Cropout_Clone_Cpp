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
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WorkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Hat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Tool;
};

enum class EVillagerJobType
{
	None,
	Idle
};
