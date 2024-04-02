// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "LevelManager.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API ULevelManager : public UManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Release() override;
};
