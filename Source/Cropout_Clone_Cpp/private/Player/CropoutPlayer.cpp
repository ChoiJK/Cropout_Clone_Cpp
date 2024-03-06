// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CropoutPlayer.h"
#include "Player/Components/MovementInputHandler.h"

#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Global/GlobalEventDispatcher.h"


// Sets default values
ACropoutPlayer::ACropoutPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocationAndRotation(FVector::Zero(), FRotator(-47.5f, 0.f, 0.f));
	SpringArm->TargetArmLength = 20000.f;
	SpringArm->SocketOffset = FVector(-300.f, 0.f, 80.f);
	SpringArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetFieldOfView(20.0f);

	CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorMesh"));
	CursorMesh->SetupAttachment(GetRootComponent());

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(GetRootComponent());

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MovementInputHandler = CreateDefaultSubobject<UMovementInputHandler>(TEXT("MovementInputHandler"));
	MovementInputHandler->Initialize(Movement);
}

// Called when the game starts or when spawned
void ACropoutPlayer::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetWorldLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));

	auto gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	if(gameInstance)
	{
		GlobalEventDispatcher = gameInstance->GetGlobalEventDispatcher();
		if(TSharedPtr<FGlobalEventDispatcher> EventDispatcher = GlobalEventDispatcher.Pin())
		{
			EventDispatcher->AddListenerUObject(EGlobalEventType::ChangedInputType, this,
			                                    &ACropoutPlayer::OnChangedInputType);
		}
	}
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

	MovementInputHandler->SetupPlayerInputComponent(PlayerInputComponent);
}

ACropoutPlayerController* ACropoutPlayer::GetPlayerController()
{
	if(PlayerController == nullptr)
	{
		PlayerController = Cast<ACropoutPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		check(PlayerController != nullptr);
	}

	return PlayerController;
}

void ACropoutPlayer::OnChangedInputType()
{
	auto currentInputType = GetPlayerController()->GetCurrentInputType();
	if(currentInputType == EInputType::Unknown)
	{
		return;
	}

	if(currentInputType == EInputType::Touch)
	{
		CursorMesh->SetHiddenInGame(true, true);
		Collision->SetRelativeLocation(FVector(0, 0, -500));
		return;
	}

	if(currentInputType == EInputType::KeyMouse)
	{
		Collision->SetRelativeLocation(FVector(0, 0, 10));
	}

	CursorMesh->SetHiddenInGame(false, true);
}
