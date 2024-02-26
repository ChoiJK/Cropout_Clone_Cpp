// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"
#include "../../public/Global/GlobalEventDispatcher.h"

void UCropoutGameInstance::Init()
{
	Super::Init();

	GlobalEventDispatcher = MakeShared<FGlobalEventDispatcher>();

	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Init" ));
}

void UCropoutGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT( "GameInstance Shutdown" ));


	Super::Shutdown();
}
