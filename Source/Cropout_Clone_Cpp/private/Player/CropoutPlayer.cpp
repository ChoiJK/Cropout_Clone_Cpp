// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CropoutPlayer.h"

#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACropoutPlayer::ACropoutPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-47.5f, 0.f, 0.f));
	SpringArm->TargetArmLength = 20000.f;
	SpringArm->SocketOffset = FVector(-300.f, 0.f, 80.f);
	SpringArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bEnableCameraLag = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetFieldOfView(20.0f);

	CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorMesh"));
	CursorMesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void ACropoutPlayer::BeginPlay()
{
	Super::BeginPlay();

	//RootComponent->SetWorldLocation(FVector::Zero());
}

// Called every frame
void ACropoutPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACropoutPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
