// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/MovementInputHandler.h"

#include "Player/CropoutPlayer.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable/InteractableBaseActor.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Chaos/SAT.h"
#include "Components/SphereComponent.h"
#include "Core/CropoutPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UMovementInputHandler::UMovementInputHandler()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imc_BaseInput
		(TEXT("EnhancedInput.InputMappingContext'/Game/Blueprint/Player/Input/IMC_BaseInput.IMC_BaseInput'"));
	if(imc_BaseInput.Succeeded())
	{
		IMC_BaseContext = imc_BaseInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Move
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_Move.IA_Move'"));
	if(ia_Move.Succeeded())
	{
		MoveAction = ia_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Zoom
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_Zoom.IA_Zoom'"));
	if(ia_Zoom.Succeeded())
	{
		ZoomAction = ia_Zoom.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Spin
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_Spin.IA_Spin'"));
	if(ia_Spin.Succeeded())
	{
		SpinAction = ia_Spin.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> c_zoom
		(TEXT("CurveFloat'/Game/Blueprint/Player/Curve/C_Zoom.C_Zoom'"));
	if(c_zoom.Succeeded())
	{
		ZoomCurve = c_zoom.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imc_dragMove
		(TEXT("EnhancedInput.InputMappingContext'/Game/Blueprint/Player/Input/IMC_DragMove.IMC_DragMove'"));
	if(imc_dragMove.Succeeded())
	{
		IMC_DragMoveContext = imc_dragMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_dragMove
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_DragMove.IA_DragMove'"));
	if(ia_dragMove.Succeeded())
	{
		DragMoveAction = ia_dragMove.Object;
	}

	check(IMC_BaseContext != nullptr);
	check(MoveAction != nullptr);
	check(ZoomAction != nullptr);
	check(SpinAction != nullptr);
	check(ZoomCurve != nullptr);
	check(IMC_DragMoveContext != nullptr);
	check(DragMoveAction != nullptr);
}

void UMovementInputHandler::Initialize(UFloatingPawnMovement* movement)
{
	Movement = movement;

	Owner = Cast<ACropoutPlayer>(GetOwner());

	check(Owner != nullptr);
}


// Called when the game starts
void UMovementInputHandler::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UMovementInputHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto playerInput = InputSubsystem->GetPlayerInput();
	int iojsdoijfwe = 0;
}

void UMovementInputHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(IMC_BaseContext == nullptr)
	{
		return;
	}

	if(const ULocalPlayer* LocalPlayer = Owner->GetPlayerController()->GetLocalPlayer())
	{
		InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if(InputSubsystem)
		{
			InputSubsystem->AddMappingContext(IMC_BaseContext, 0);
			// @TODO : Villiger가 추가되면 DragMoveIMC의 초기화를 여기서 안해도 됨
			InitDragMoveIMC();
			if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnMovePressed);
				EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnZoomPressed);
				EnhancedInputComponent->BindAction(SpinAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnSpinPressed);
				EnhancedInputComponent->BindAction(DragMoveAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnDragMovePressed);
			}
		}
	}
}

void UMovementInputHandler::InitDragMoveIMC() const
{
	if(InputSubsystem)
	{
		InputSubsystem->AddMappingContext(IMC_DragMoveContext, 0);
	}
}

void UMovementInputHandler::ReleaseDragMoveIMC() const
{
	if(InputSubsystem)
	{
		FModifyContextOptions options;
		options.bIgnoreAllPressedKeysUntilRelease = true;
		options.bForceImmediately = true;
		options.bNotifyUserSettings = false;

		InputSubsystem->RemoveMappingContext(IMC_DragMoveContext, options);
	}
}

void UMovementInputHandler::MoveTracking()
{
	auto actorLocation = Owner->GetActorLocation();
	float locationLength = actorLocation.Length();
	auto normalizedVec = actorLocation.Normalize();

	actorLocation.Z = 0.f;
	actorLocation = -actorLocation;

	locationLength = FMath::Max(0.f, (locationLength - 9000.f) / 5000.f);

	Owner->AddMovementInput(actorLocation, locationLength);

	UpdateCursorPosition();

	FVector worldDirection;
	float scaleVal;
	GetEdgeMove(worldDirection, scaleVal);

	Owner->AddMovementInput(worldDirection, scaleVal);

	FVector2D ScreenPos;
	FVector intersection;
	ProjectTouchToGroundPlane(ScreenPos, intersection);
	intersection.Z += 10.f;
	Owner->Collision->SetWorldLocation(intersection);
}


void UMovementInputHandler::OnMovePressed(const FInputActionValue& Value)
{
	Owner->AddMovementInput(Owner->GetActorForwardVector(), Value[1]);
	Owner->AddMovementInput(Owner->GetActorRightVector(), Value[0]);
}

void UMovementInputHandler::OnSpinPressed(const FInputActionValue& Value)
{
	Owner->AddActorLocalRotation(FRotator(0, Value[0], 0));
}

void UMovementInputHandler::OnZoomPressed(const FInputActionValue& Value)
{
	ZoomDirection = Value[0];
	UpdateZoom();
	UpdateDof();
}

void UMovementInputHandler::OnDragMovePressed(const FInputActionValue& Value)
{
	if(SingleTouchCheck())
	{
		TrackMove();
	}
	else
	{
		ReleaseDragMoveIMC();
	}
}

void UMovementInputHandler::UpdateZoom()
{
	ZoomValue += ZoomDirection * 0.01f;
	ZoomValue = FMath::Clamp(ZoomValue, 0.0f, 1.f);
	float lerpKey = ZoomCurve != nullptr ? ZoomCurve->GetFloatValue(ZoomValue) : 0.5f;

	Owner->SpringArm->TargetArmLength = FMath::Lerp(800.f, 40000.f, lerpKey);
	Owner->SpringArm->SetRelativeRotation(FRotator(FMath::Lerp(-40.0f, -55.0f, lerpKey), 0.f, 0.f));

	Owner->Movement->MaxSpeed = FMath::Lerp(1000.f, 6000.f, lerpKey);

	Owner->Camera->FieldOfView = FMath::Lerp(20.f, 15.f, lerpKey);
}

void UMovementInputHandler::UpdateDof() const
{
	Owner->Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	Owner->Camera->PostProcessSettings.bOverride_DepthOfFieldFstop = 3.f;
	Owner->Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = 150.f;
	Owner->Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = Owner->SpringArm->TargetArmLength;
}

bool UMovementInputHandler::SingleTouchCheck() const
{
	float locX, locY;
	bool isCurrentlyTouching = false;
	Owner->GetPlayerController()->GetInputTouchState(ETouchIndex::Touch2, locX, locY, isCurrentlyTouching);

	return !isCurrentlyTouching;
}

void UMovementInputHandler::TrackMove()
{
	FVector2D ScreenPos;
	FVector IntersectionPos;
	if(ProjectTouchToGroundPlane(ScreenPos, IntersectionPos))
	{
		// @Todo클릭시 TargetHandle을 초기화 (이건 Villiger 이벤트에서 처리됨)

		FVector offset;
		FVector forwardVec = Owner->SpringArm->GetForwardVector();
		FVector upVec = Owner->SpringArm->GetUpVector();
		FVector socketOffset = Owner->SpringArm->SocketOffset;
		FVector springArmWorldLoc = Owner->SpringArm->GetComponentLocation();
		FVector cameraWorldLoc = Owner->Camera->GetComponentLocation();

		FVector offsetApplyForwardVec = -(forwardVec * (Owner->SpringArm->TargetArmLength - socketOffset.X));
		offsetApplyForwardVec = offsetApplyForwardVec + (upVec * socketOffset.Z) + springArmWorldLoc;
		offset = offsetApplyForwardVec - cameraWorldLoc;

		StoredMove = TargetHandle - IntersectionPos - offset;

		Owner->AddActorWorldOffset(FVector(StoredMove.X, StoredMove.Y, 0.f));
	}
}

bool UMovementInputHandler::ProjectTouchToGroundPlane(FVector2D& ScreenPos, FVector& IntersectionPos)
{
	auto playerController = Owner->GetPlayerController();

	FVector2D screenPos = GetViewportCenter();

	bool hasTouch = false;
	float worldLocZ = 0.f;
	switch(playerController->GetCurrentInputType())
	{
	case EInputType::KeyMouse:
		double mouseX, mouseY;
		if(playerController->GetMousePosition(mouseX, mouseY))
		{
			hasTouch = true;
			screenPos.X = mouseX;
			screenPos.Y = mouseY;
		}
		break;
	case EInputType::GamePad:
		hasTouch = true;
	case EInputType::Touch:
		double touchX, touchY;
		bool isCurrentlyTouching;
		playerController->GetInputTouchState(ETouchIndex::Touch1, touchX, touchY, isCurrentlyTouching);
		if(isCurrentlyTouching)
		{
			hasTouch = true;
			screenPos.X = touchX;
			screenPos.Y = touchY;
		}
		else
		{
			worldLocZ = -500.f;
		}
		break;
	}

	FVector worldLocation, worldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(
		screenPos.X, screenPos.Y, worldLocation, worldDirection);
	FVector end = worldLocation + (worldDirection * 100000.0f);

	FVector intersectionLoc =
		FMath::LinePlaneIntersection(worldLocation, end, FVector(0, 0, 0), FVector(0, 0, 1));

	ScreenPos = screenPos;
	intersectionLoc.Z += worldLocZ;
	IntersectionPos = intersectionLoc;
	return hasTouch;
}

void UMovementInputHandler::UpdateCursorPosition()
{
	auto currentInputType = Owner->GetPlayerController()->GetCurrentInputType();
	if(currentInputType == EInputType::Unknown || currentInputType == EInputType::Touch)
	{
		return;
	}

	FTransform targetTransform;
	AActor* targetActor = Cast<AActor>(Owner->GetHoverActor());
	if(targetActor == nullptr)
	{
		// 지면
		targetTransform = FTransform(Owner->Collision->GetComponentRotation(),
		                             Owner->Collision->GetComponentLocation(),
		                             FVector(2.f, 2.f, 1.f));
	}
	else
	{
		// 마우스 포인트에 위치한 객체가 존재함
		FVector boundOrigin;
		FVector boundExtent;
		targetActor->GetActorBounds(true, boundOrigin, boundExtent);

		boundOrigin.Z = 20.f;
		float boundExtentMax = FVector2D(boundExtent.X, boundExtent.Y).GetAbsMax() / 50.f;
		float timeOffset = FMath::Sin(GetWorld()->GetTimeSeconds() * 0.5f) * 0.25f;
		float selectedScale = boundExtentMax + timeOffset + 1.f;

		targetTransform = FTransform(FRotator::ZeroRotator,
		                             boundOrigin,
		                             FVector(selectedScale, selectedScale, 1.f));
	}

	// 시간의 흐름에 따라 부드럽게 이동 및 Scale 적용
	// @Todo : 빠릿빠릿하다고 느껴지지 않으면 Scale만 분리해서 적용해도 괜찮을듯
	auto interpTransform = UKismetMathLibrary::TInterpTo(Owner->CursorMesh->GetComponentTransform(), targetTransform,
	                                                     GetWorld()->GetDeltaSeconds(), 12.f);
	Owner->CursorMesh->SetWorldTransform(interpTransform);
}

void UMovementInputHandler::GetEdgeMove(FVector& Direction, float& Strength)
{
	FVector2D ScreenPos;
	FVector IntersectionPos;
	ProjectTouchToGroundPlane(ScreenPos, IntersectionPos);
	FVector2D viewportCenter = GetViewportCenter();

	FVector temDirection;
	CursorDistFromViewportCenter(ScreenPos - viewportCenter, temDirection, Strength);
	Direction = Owner->GetActorTransform().TransformVectorNoScale(temDirection);
}

FVector2D UMovementInputHandler::GetViewportCenter() const
{
	auto playerController = Owner->GetPlayerController();

	int32 viewportSizeX, viewportSizeY;
	playerController->GetViewportSize(viewportSizeX, viewportSizeY);

	FVector2D screenPos;
	screenPos.X = viewportSizeX / 2.f;
	screenPos.Y = viewportSizeY / 2.f;

	return screenPos;
}

void UMovementInputHandler::CursorDistFromViewportCenter(FVector2D mousePosFromViewportCenter, FVector& Direction,
                                                         float& Strength) const
{
	FVector2D edgeDetectDistance = GetViewportCenter();

	switch(Owner->GetPlayerController()->GetCurrentInputType())
	{
	case EInputType::KeyMouse:
		edgeDetectDistance.X -= EdgeMoveDistance;
		edgeDetectDistance.Y -= EdgeMoveDistance;
		break;
	case EInputType::GamePad:
		edgeDetectDistance.X -= EdgeMoveDistance * 2.f;
		edgeDetectDistance.Y -= EdgeMoveDistance * 2.f;
		break;
	case EInputType::Touch:
		edgeDetectDistance.X -= EdgeMoveDistance * 2.f;
		edgeDetectDistance.Y -= EdgeMoveDistance * 2.f;
		break;
	}

	FVector2D absPos = mousePosFromViewportCenter.GetAbs();
	absPos -= edgeDetectDistance;
	absPos = FVector2D::Max(absPos, FVector2D::ZeroVector);
	absPos /= EdgeMoveDistance;

	FVector2D signVec = FVector2D(FMath::Sign(mousePosFromViewportCenter.X) * absPos.X,
	                              FMath::Sign(mousePosFromViewportCenter.Y) * absPos.Y * -1.f);
	Direction = FVector(signVec.Y, signVec.X, 0.f);
	Strength = 1.f;
}
