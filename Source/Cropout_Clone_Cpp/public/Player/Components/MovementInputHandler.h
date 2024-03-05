// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementInputHandler.generated.h"

class APostProcessVolume;
class ACropoutPlayer;
class UCurveFloat;
class UInputMappingContext;
class UFloatingPawnMovement;
class UInputAction;
class UInputComponent;

struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROPOUT_CLONE_CPP_API UMovementInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMovementInputHandler();

	void Initialize(UFloatingPawnMovement* movement);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	ACropoutPlayer* Owner = nullptr;

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
	void UpdateDof() const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
};
