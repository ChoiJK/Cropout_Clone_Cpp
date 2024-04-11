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

	FName ManagerName;

	void SetManagerName(FName name)
	{
		ManagerName = name;
	}

	bool IsRelease = false;
	bool IsDestroy = false;

public:
	void SetGameInstance(UCropoutGameInstance* instance);

	FString GetManagerName() const
	{
		return ManagerName.ToString();
	}

	virtual void Init()
	{
	}

	virtual void Prepare()
	{
	}

	virtual void Release()
	{
		IsRelease = true;
	}

	virtual void Destroy()
	{
		IsDestroy = true;
	}

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;
};
