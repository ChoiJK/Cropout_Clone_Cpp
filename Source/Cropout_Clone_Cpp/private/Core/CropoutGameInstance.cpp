// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"
#include "Global/GlobalEventDispatcher.h"
#include "Global/GlobalSharedVariable.h"
#include "Data/SaveData.h"

void UCropoutGameInstance::Init()
{
	Super::Init();

	GlobalEventDispatcher = MakeShared<FGlobalEventDispatcher>();
	GlobalSharedVariable = MakeShared<FGlobalSharedVariable>(this);

	SaveData = NewObject<USaveData>();

	InitializeSystemManagers();
}

void UCropoutGameInstance::FinishDestroy()
{
	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Release();
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Destroy();
	}
	Super::FinishDestroy();
}

void UCropoutGameInstance::InitializeSystemManagers()
{
	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(USystemManager::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			USystemManager* SystemManager = NewObject<USystemManager>(this, Class);
			SystemManager->SetGameInstance(this);
			SystemManagerMap.Add(Class, SystemManager);
		}
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Init();
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Prepare();
	}
}
