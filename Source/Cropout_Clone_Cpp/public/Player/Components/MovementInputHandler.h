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
class UEnhancedInputLocalPlayerSubsystem;

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
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;

	UFloatingPawnMovement* Movement = nullptr;
	UInputMappingContext* IMC_BaseContext = nullptr;
	UInputAction* MoveAction = nullptr;
	UInputAction* ZoomAction = nullptr;
	UInputAction* SpinAction = nullptr;

	UInputMappingContext* IMC_DragMoveContext = nullptr;
	UInputAction* DragMoveAction = nullptr;

	void OnMovePressed(const FInputActionValue& Value);
	void OnSpinPressed(const FInputActionValue& Value);
	void OnZoomPressed(const FInputActionValue& Value);
	void OnDragMovePressed(const FInputActionValue& Value);

	// Zoom
	float ZoomDirection = 0.f;
	float ZoomValue = 0.5f;
	UCurveFloat* ZoomCurve = nullptr;

	// DragMove
	FVector StoredMove;
	FVector TargetHandle;
	bool SingleTouchCheck() const;
	void TrackMove();
	bool ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos);

	// Cursor
	float EdgeMoveDistance = 50.0f;
	void UpdateCursorPosition();
	void GetEdgeMove(FVector& Direction, float& Strength);
	FVector2D GetViewportCenter() const;
	void CursorDistFromViewportCenter(FVector2D mousePosFromViewportCenter, FVector& Direction, float& Strength) const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void InitDragMoveIMC() const;
	void ReleaseDragMoveIMC() const;

	void MoveTracking();
	void UpdateZoom();
	void UpdateDof() const;
};
