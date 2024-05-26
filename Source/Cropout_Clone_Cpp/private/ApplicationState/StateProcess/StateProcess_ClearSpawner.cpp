// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplicationState/StateProcess/StateProcess_ClearSpawner.h"

#include "Core/CropoutGameInstance.h"
#include "Entity/EntityManager.h"
#include "Spawn/Spawner.h"

void UStateProcess_ClearSpawner::Init()
{
}

void UStateProcess_ClearSpawner::Process(UStateProcessSharedData* sharedData)
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	AInGameGameMode* gameMode = gameInstance->GetCurrentInGameMode();
	if(gameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("gameMode is nullptr"));
		return;
	}
	UEntityManager* entityManager = gameInstance->GetLevelManager<UEntityManager>();

	ASpawner* spawner = entityManager->GetSpawner();
	if(spawner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawner is nullptr"));
		return;
	}

	spawner->ClearSpawnerTimer();
}

bool UStateProcess_ClearSpawner::IsSuccess()
{
	return true;
}
