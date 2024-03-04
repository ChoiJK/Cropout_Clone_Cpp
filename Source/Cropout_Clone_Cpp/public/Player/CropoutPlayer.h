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
class UInputMappingContext;
class UInputAction;
class UCurveFloat;
struct FInputActionValue;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayer : public APawn
{
	GENERATED_BODY()

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

	UInputMappingContext* IMC_BaseContext = nullptr;
	UInputAction* MoveAction = nullptr;
	UInputAction* ZoomAction = nullptr;
	UInputAction* SpinAction = nullptr;

	void OnMovePressed(const FInputActionValue& Value);
	void OnSpinPressed(const FInputActionValue& Value);
	void OnZoomPressed(const FInputActionValue& Value);

	float ZoomDirection = 0.f;
	float ZoomValue = 0.5f;
	UCurveFloat* ZoomCurve = nullptr;
	void UpdateZoom();
	void UpdateDof();

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
