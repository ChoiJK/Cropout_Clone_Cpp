// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_SetGamePausedMode.h"

#include "Core/CropoutGameInstance.h"

void UStateProcess_SetGamePausedMode::Init()
{
}

void UStateProcess_SetGamePausedMode::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	APlayerController* PC = gameInstance->GetCurrentPlayerController();
	PC->SetPause(IsPaused);
}

bool UStateProcess_SetGamePausedMode::IsSuccess()
{
	return true;
}

void UStateProcess_SetGamePausedMode::SetPaused(bool bPaused)
{
	IsPaused = bPaused;
}
