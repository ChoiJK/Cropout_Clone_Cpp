// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/IslandBaseGameMode.h"
#include "CropoutMainMenuGameMode.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutMainMenuGameMode : public AIslandBaseGameMode
{
	GENERATED_BODY()

public:
	ACropoutMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
};
