// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ManagerBase.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UManagerBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize()
	{
	}

	virtual void Release()
	{
	}
};
