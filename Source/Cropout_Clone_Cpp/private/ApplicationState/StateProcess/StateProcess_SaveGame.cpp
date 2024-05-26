// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_SaveGame.h"

#include "Core/CropoutGameInstance.h"

void UStateProcess_SaveGame::Init()
{
}

void UStateProcess_SaveGame::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance::GetInstance()->SaveInGameData();
}

bool UStateProcess_SaveGame::IsSuccess()
{
	return true;
}
