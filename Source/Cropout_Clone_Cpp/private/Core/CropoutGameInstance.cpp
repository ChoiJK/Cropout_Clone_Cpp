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

	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Init" ));
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Shutdown" ));

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Release();
	}

	Super::Shutdown();
}

void UCropoutGameInstance::InitializeSystemManagers()
{
	for(TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if(Class->IsChildOf(USystemManager::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
			SystemManagerMap.Add(Class, NewObject<USystemManager>(this, Class));
		}
	}

	for(auto& Pair : SystemManagerMap)
	{
		Pair.Value->Initialize();
	}
}
