// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/InGame/InGameGameMode.h"

#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGamePlayerController.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Player/CropoutPlayer.h"
#include "Enum/ResourceType.h"

AInGameGameMode::AInGameGameMode()
{
	DefaultPawnClass = ACropoutPlayer::StaticClass();
	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @ TODO : ScreenEffect FadeOut

	// 다음 프레임에 적용 (GameMode와 LevelScriptActor의 생성 순서 때문)
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		StoreResource(EResourceType::Food, 100);
	}));
}

void AInGameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

int AInGameGameMode::GetResourceAmount(EResourceType type)
{
	return ResourceBank.Contains(type) ? ResourceBank[type]->GetValue() : -1;
}

bool AInGameGameMode::HasResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type]->GetValue() >= amount)
	{
		return true;
	}

	return false;
}

void AInGameGameMode::StoreResource(EResourceType type, int amount)
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

void AInGameGameMode::SpendResource(EResourceType type, int amount)
{
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - amount);

		if(IsContinueGame == false && ResourceBank[type]->GetValue() < 0 && type == EResourceType::Food)
		{
			UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}
}

bool AInGameGameMode::ExtractResource(EResourceType type, int requestedAmount, int& withdrawnAmount)
{
	bool isWithdrawn = false;
	if(ResourceBank.Contains(type) && ResourceBank[type] != nullptr)
	{
		isWithdrawn = true;
		withdrawnAmount = requestedAmount;
		ResourceBank[type]->SetValue(ResourceBank[type]->GetValue() - requestedAmount);

		if(IsContinueGame == false && ResourceBank[type]->GetValue() < 0 && type == EResourceType::Food)
		{
			UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::LoseGame);
		}
	}

	return isWithdrawn;
}

std::shared_ptr<SimpleRxReactiveProperty<int>> AInGameGameMode::GetResourceRP(EResourceType type)
{
	if(ResourceBank.Contains(type) == false)
	{
		ResourceBank.Add(type, std::make_shared<SimpleRxReactiveProperty<int>>(0));
	}

	return ResourceBank[type];
}

void AInGameGameMode::ResourceDebugMessage()
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
