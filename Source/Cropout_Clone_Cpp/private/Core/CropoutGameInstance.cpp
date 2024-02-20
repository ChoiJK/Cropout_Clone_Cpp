// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutGameInstance.h"

void UCropoutGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
}

void UCropoutGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Shutdown"));
}
