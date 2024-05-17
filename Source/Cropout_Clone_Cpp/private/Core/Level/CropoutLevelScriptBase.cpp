// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Level/CropoutLevelScriptBase.h"
#include "Manager/Base/LevelManager.h"
#include "Core/CropoutGameInstance.h"
#include "Core/ApplicationState/ApplicationStateManager.h"


ACropoutLevelScriptBase::ACropoutLevelScriptBase()
{
}

void ACropoutLevelScriptBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
	ASM->UpdateState();
}

void ACropoutLevelScriptBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	gameInstance->SetCurrentLevelScript(this);
}

void ACropoutLevelScriptBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACropoutLevelScriptBase::BeginDestroy()
{
	Super::BeginDestroy();

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	if(gameInstance != nullptr)
	{
		gameInstance->SetCurrentLevelScript(nullptr);
	}
}

void ACropoutLevelScriptBase::Destroyed()
{
	Super::Destroyed();
}
