// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/CropoutLevelScriptBase.h"
#include "Manager/Base/LevelManager.h"


ACropoutLevelScriptBase::ACropoutLevelScriptBase()
{
}


void ACropoutLevelScriptBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeLevelManagers();
}

void ACropoutLevelScriptBase::Destroyed()
{
	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Release();
	}

	Super::Destroyed();
}

void ACropoutLevelScriptBase::InitializeLevelManagers()
{
	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(ULevelManager::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			LevelManagerMap.Add(Class, NewObject<ULevelManager>(this, Class));
		}
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Initialize();
	}
}
