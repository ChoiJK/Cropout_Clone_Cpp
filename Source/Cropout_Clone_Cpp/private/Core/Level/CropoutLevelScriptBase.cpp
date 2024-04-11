// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/CropoutLevelScriptBase.h"
#include "Manager/Base/LevelManager.h"
#include "Core/CropoutGameInstance.h"


ACropoutLevelScriptBase::ACropoutLevelScriptBase()
{
}

void ACropoutLevelScriptBase::BeginPlay()
{
	Super::BeginPlay();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	gameInstance->SetCurrentLevelScript(this);

	if(IsManagerInitialized == false)
	{
		InitializeLevelManagers();
	}
}

void ACropoutLevelScriptBase::Destroyed()
{
	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Release();
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Destroy();
	}

	Super::Destroyed();
}

void ACropoutLevelScriptBase::InitializeLevelManagers()
{
	IsProgressManagerInitialized = true;

	UCropoutGameInstance* gi = UCropoutGameInstance::GetInstance();

	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(ULevelManager::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			ULevelManager* LevelManager = NewObject<ULevelManager>(this, Class);
			LevelManager->SetGameInstance(gi);
			LevelManagerMap.Add(Class, LevelManager);
		}
	}

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Init();
	}

	IsManagerInitialized = true;
	IsProgressManagerInitialized = false;

	for(auto& Pair : LevelManagerMap)
	{
		Pair.Value->Prepare();
	}
}
