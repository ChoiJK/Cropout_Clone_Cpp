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
}

void ACropoutLevelScriptBase::Destroyed()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	gameInstance->SetCurrentLevelScript(nullptr);


	Super::Destroyed();
}
