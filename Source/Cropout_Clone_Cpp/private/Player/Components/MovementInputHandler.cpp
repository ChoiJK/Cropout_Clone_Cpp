// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/MovementInputHandler.h"

#include "Player/CropoutPlayer.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

UMovementInputHandler::UMovementInputHandler()
{
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

	check(IMC_BaseContext != nullptr);
	check(MoveAction != nullptr);
	check(ZoomAction != nullptr);
	check(SpinAction != nullptr);
	check(ZoomCurve != nullptr);
}

void UMovementInputHandler::Initialize(UFloatingPawnMovement* movement)
{
	Owner = Cast<ACropoutPlayer>(GetOwner());
	Movement = movement;

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
}

void UMovementInputHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(IMC_BaseContext == nullptr)
	{
		return;
	}

	auto PC = UGameplayStatics::GetPlayerController(this, 0);

	if(const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(IMC_BaseContext, 1);
			if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnMovePressed);
				EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnZoomPressed);
				EnhancedInputComponent->BindAction(SpinAction, ETriggerEvent::Triggered, this,
				                                   &UMovementInputHandler::OnSpinPressed);
			}
		}
	}
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
