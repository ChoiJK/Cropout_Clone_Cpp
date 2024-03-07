// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Villager.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API AVillager : public APawn
{
	GENERATED_BODY()

public:
	AVillager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Action(AActor* jobAction);
};
