// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EnvQueryGenerator_SimpleGridWithResourceTarget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Points: Grid_ResourceTarget"))
class CROPOUT_CLONE_CPP_API UEnvQueryGenerator_SimpleGridWithResourceTarget : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_UCLASS_BODY()
	/** half of square's extent, like a radius */
	UPROPERTY(EditDefaultsOnly, Category=Generator, meta=(DisplayName="GridHalfSize"))
	FAIDataProviderFloatValue GridSize;

	/** generation density */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue SpaceBetween;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
