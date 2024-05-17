// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/IslandBaseGameMode.h"
#include "Enum/ResourceType.h"
#include "InGameGameMode.generated.h"

class ABuildingBaseActor;
class AVillager;

UCLASS()
class CROPOUT_CLONE_CPP_API AInGameGameMode : public AIslandBaseGameMode
{
	GENERATED_BODY()

	bool IsContinueGame = false;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AInGameGameMode();

	void SetIsContinueGame(bool value) { IsContinueGame = value; }
	bool GetIsContinueGame() const { return IsContinueGame; }
};
