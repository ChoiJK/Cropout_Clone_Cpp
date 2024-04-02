// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/SystemManager.h"
#include "UObject/Object.h"
#include "TableManager.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UTableManager : public USystemManager
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Release() override;
};
