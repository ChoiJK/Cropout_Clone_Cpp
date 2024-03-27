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

	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Init" ));
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Shutdown" ));


	Super::Shutdown();
}
