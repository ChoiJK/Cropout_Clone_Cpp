// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Manager/Base/LevelManager.h"
#include "CropoutLevelScriptBase.generated.h"

class ULevelManager;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutLevelScriptBase : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACropoutLevelScriptBase();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void Destroyed() override;
};
