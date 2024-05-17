// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ResourceItemManager.h"

#include "Core/CropoutGameInstance.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Core/InGame/InGameGameMode.h"
#include "Enum/ResourceType.h"


void UResourceItemManager::Init()
{
	Super::Init();
}

void UResourceItemManager::Prepare()
{
	Super::Prepare();
}

void UResourceItemManager::Release()
{
	Super::Release();
}

void UResourceItemManager::Destroy()
{
	Super::Destroy();
}

void UResourceItemManager::OnSaveData(UInGameSaveData* SaveData)
{
	Super::OnSaveData(SaveData);
}

void UResourceItemManager::OnLoadData(UInGameSaveData* SaveData)
{
	Super::OnLoadData(SaveData);
}

int UResourceItemManager::GetResourceAmount(EResourceType type)
{
	return ResourceBank.Contains(type) ? ResourceBank[type]->GetValue() : -1;
}

bool UResourceItemManager::HasResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type]->GetValue() >= amount)
	{
		return true;
	}

	return false;
}

void UResourceItemManager::StoreResource(EResourceType type, int amount)
{
	if(!ResourceBank.Contains(type))
	{
		ResourceBank.Add(type, std::make_shared<SimpleRxReactiveProperty<int>>(amount));
	}
	else
	{
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() + amount);
	}
}

void UResourceItemManager::SpendResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - amount);

		UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
		if(gameInstance->GetCurrentInGameMode()->GetIsContinueGame() == false && ResourceBank[type]->GetValue() < 0 &&
			type == EResourceType::Food)
		{
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}
}

bool UResourceItemManager::ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount)
{
	bool isWithdrawn = false;
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		isWithdrawn = true;
		withdrawnAmount = requestedAmount;
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - requestedAmount);

		UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
		if(gameInstance->GetCurrentInGameMode()->GetIsContinueGame() == false && ResourceBank[type]->GetValue() < 0 &&
			type == EResourceType::Food)
		{
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}

	return isWithdrawn;
}

std::shared_ptr<SimpleRxReactiveProperty<int>> UResourceItemManager::GetResourceRP(EResourceType type)
{
	if(ResourceBank.Contains(type) == false)
	{
		ResourceBank.Add(type, std::make_shared<SimpleRxReactiveProperty<int>>(0));
	}

	return ResourceBank[type];
}


void UResourceItemManager::ResourceDebugMessage()
{
	if(ResourceBank.Contains(EResourceType::Food))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Food), 5.f, FColor::Green,
			FString::Printf(TEXT("Food %d"), ResourceBank[EResourceType::Food]->GetValue()));
	}

	if(ResourceBank.Contains(EResourceType::Wood))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Wood), 5.f, FColor::Red,
			FString::Printf(TEXT("Wood %d"), ResourceBank[EResourceType::Wood]->GetValue()));
	}

	if(ResourceBank.Contains(EResourceType::Stone))
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EResourceType::Stone), 5.f, FColor::Black,
			FString::Printf(TEXT("Stone %d"), ResourceBank[EResourceType::Stone]->GetValue()));
	}
}
