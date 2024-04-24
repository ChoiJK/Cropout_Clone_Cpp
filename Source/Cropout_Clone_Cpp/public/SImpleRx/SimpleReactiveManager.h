// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Base/SystemManager.h"
#include "SimpleReactiveManager.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API USimpleReactiveManager : public USystemManager
{
	GENERATED_BODY()

	static USimpleReactiveManager* Instance;

public:
	static USimpleReactiveManager* GetInstance() { return Instance; }

protected:
	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

public:
};
