// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_SetGameContinue.h"

#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGameGameMode.h"

void UStateProcess_SetGameContinue::Init()
{
}

void UStateProcess_SetGameContinue::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	AInGameGameMode* gameMode = gameInstance->GetCurrentInGameMode();
	gameMode->SetIsContinueGame(true);
}

bool UStateProcess_SetGameContinue::IsSuccess()
{
	return true;
}
