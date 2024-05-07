// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMenu/MainMenuPawn.h"
#include "Core/MainMenu/CameraShakeIdle.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocationAndRotation(FVector::Zero(), FRotator(-30.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 7800.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetFieldOfView(20.0f);

	CameraShakeSource = CreateDefaultSubobject<UCameraShakeSourceComponent>(TEXT("CameraShakeSource"));
	CameraShakeSource->SetupAttachment(GetRootComponent());
	CameraShakeSource->Attenuation = ECameraShakeAttenuation::Linear;
	CameraShakeSource->InnerAttenuationRadius = 100000.f;
	CameraShakeSource->OuterAttenuationRadius = 100000.f;
	CameraShakeSource->CameraShake = UCameraShakeIdle::StaticClass();
	CameraShakeSource->bAutoStart = true;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.f, 5.0f, 0.f);
}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
