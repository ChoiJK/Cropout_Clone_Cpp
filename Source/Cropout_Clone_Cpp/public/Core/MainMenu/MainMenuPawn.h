// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"

class UCameraShakeSourceComponent;
class USpringArmComponent;
class UCameraComponent;
class URotatingMovementComponent;

UCLASS()
class CROPOUT_CLONE_CPP_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UCameraShakeSourceComponent* CameraShakeSource = nullptr;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere)
	URotatingMovementComponent* RotatingMovement = nullptr;

public:
	// Sets default values for this pawn's properties
	AMainMenuPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
