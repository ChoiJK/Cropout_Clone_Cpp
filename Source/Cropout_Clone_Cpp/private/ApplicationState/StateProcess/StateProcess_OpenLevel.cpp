// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_OpenLevel.h"

#include "Core/CropoutGameInstance.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"


void UStateProcess_OpenLevel::Init()
{
	isOpenProcess = false;
	isOpenSuccess = false;
	if(OnPostLoadMapWithWorldDelegateHandle.IsValid())
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(OnPostLoadMapWithWorldDelegateHandle);
	}
}

void UStateProcess_OpenLevel::Process(UStateProcessSharedData* sharedData)
{
	if(LevelName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("LevelName is empty"));
		return;
	}

	if(isOpenProcess == false)
	{
		UGameplayStatics::OpenLevel(UCropoutGameInstance::GetInstance(), FName(LevelName));
		OnPostLoadMapWithWorldDelegateHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,
			&UStateProcess_OpenLevel::OnPostLoadMapWithWorld);
		isOpenProcess = true;
	}
}

void UStateProcess_OpenLevel::OnPostLoadMapWithWorld(UWorld* LoadedWorld)
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(OnPostLoadMapWithWorldDelegateHandle);

	isOpenSuccess = true;
}

bool UStateProcess_OpenLevel::IsSuccess()
{
	return isOpenSuccess;
}

void UStateProcess_OpenLevel::SetLevelName(const FString& String)
{
	LevelName = String;
}
