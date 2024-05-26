// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_SetInputMode.h"

#include "Core/CropoutGameInstance.h"

void UStateProcess_SetInputMode::Init()
{
}

void UStateProcess_SetInputMode::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	APlayerController* PC = gameInstance->GetCurrentPlayerController();
	if(GameInputMode && !UIInputMode)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
	else if(!GameInputMode && UIInputMode)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}
	else if(GameInputMode && UIInputMode)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = false;
	}
}

bool UStateProcess_SetInputMode::IsSuccess()
{
	return true;
}

void UStateProcess_SetInputMode::SetInputMode(bool bUIInputMode, bool bGameInputMode)
{
	UIInputMode = bUIInputMode;
	GameInputMode = bGameInputMode;
}
