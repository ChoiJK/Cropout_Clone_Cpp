// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ManagerBase.generated.h"

class UCropoutGameInstance;

UCLASS()
class CROPOUT_CLONE_CPP_API UManagerBase : public UObject
{
	GENERATED_BODY()

protected:
	UCropoutGameInstance* GameInstance;

public:
	void SetGameInstance(UCropoutGameInstance* instance);

	virtual void Init()
	{
	}

	virtual void Prepare()
	{
	}

	virtual void Release()
	{
	}

	virtual void Destroy()
	{
	}
};
