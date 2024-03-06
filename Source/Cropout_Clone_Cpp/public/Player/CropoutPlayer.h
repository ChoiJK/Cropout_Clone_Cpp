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
class UMovementInputHandler;
class FGlobalEventDispatcher;
class ACropoutPlayerController;
class AInteractableBaseActor;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayer : public APawn
{
	GENERATED_BODY()

public:
	ACropoutPlayer();

private:
	TWeakPtr<FGlobalEventDispatcher> GlobalEventDispatcher = nullptr;
	ACropoutPlayerController* PlayerController = nullptr;

	AInteractableBaseActor* HoverActor = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ACropoutPlayerController* GetPlayerController();
	void OnChangedInputType();

	void SetHoverActor(AInteractableBaseActor* actor) { HoverActor = actor; }
	AInteractableBaseActor* GetHoverActor() { return HoverActor; }

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CursorMesh = nullptr;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collision = nullptr;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement = nullptr;

	UMovementInputHandler* MovementInputHandler = nullptr;
};
