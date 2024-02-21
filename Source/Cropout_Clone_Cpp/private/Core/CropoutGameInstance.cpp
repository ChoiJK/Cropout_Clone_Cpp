// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"
#include "Global/GlobalEventDispatcher.h"

void UCropoutGameInstance::Init()
{
	Super::Init();

	globalEventDispatcher = MakeShared<GlobalEventDispatcher>();

	UE_LOG( LogTemp, Warning, TEXT( "GameInstance Init" ) );
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG( LogTemp, Warning, TEXT( "GameInstance Shutdown" ) );


	Super::Shutdown();
}
