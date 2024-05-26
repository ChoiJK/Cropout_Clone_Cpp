// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_ClearSaveGame.h"

#include "Core/CropoutGameInstance.h"

void UStateProcess_ClearSaveGame::Init()
{
}

void UStateProcess_ClearSaveGame::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance::GetInstance()->ClearInGameData();
}

bool UStateProcess_ClearSaveGame::IsSuccess()
{
	return true;
}
