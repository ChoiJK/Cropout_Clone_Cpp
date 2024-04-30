// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CropoutLevelScriptBase.h"
#include "SplashLevel.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API ASplashLevel : public ACropoutLevelScriptBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
};
