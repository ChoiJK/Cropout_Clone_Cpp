// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CropoutPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USphereComponent;
class UFloatingPawnMovement;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayer : public APawn
{
	GENERATED_BODY()

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	UStaticMeshComponent* CursorMesh;
	USphereComponent* Collision;
	UFloatingPawnMovement* Movement;

public:
	// Sets default values for this pawn's properties
	ACropoutPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
