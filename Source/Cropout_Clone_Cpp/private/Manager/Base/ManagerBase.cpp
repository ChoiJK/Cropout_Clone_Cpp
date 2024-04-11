// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Base/ManagerBase.h"

void UManagerBase::SetGameInstance(UCropoutGameInstance* instance)
{
	GameInstance = instance;
}

void UManagerBase::BeginDestroy()
{
	Super::BeginDestroy();

	if(OnBeginDestroyDelegate.IsValid())
	{
		OnBeginDestroyDelegate->Broadcast(this);
		OnBeginDestroyDelegate.Reset();
	}
}
