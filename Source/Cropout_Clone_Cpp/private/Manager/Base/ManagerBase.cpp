// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Base/ManagerBase.h"

void UManagerBase::SetGameInstance(UCropoutGameInstance* instance)
{
	GameInstance = instance;
}

void UManagerBase::BeginDestroy()
{
	Super::BeginDestroy();
	if(IsRelease)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Release"), *GetManagerName());
		Release();
	}
}

void UManagerBase::FinishDestroy()
{
	Super::FinishDestroy();
	if(IsDestroy)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Destroy"), *GetManagerName());
		Destroy();
	}
}
