// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CropoutPlayer.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/EngineTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


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
}

// Called when the game starts or when spawned
void ACropoutPlayer::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetWorldLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
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
				                                   &ACropoutPlayer::OnMovePressed);
				EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this,
				                                   &ACropoutPlayer::OnZoomPressed);
				EnhancedInputComponent->BindAction(SpinAction, ETriggerEvent::Triggered, this,
				                                   &ACropoutPlayer::OnSpinPressed);
			}
		}
	}
}

void ACropoutPlayer::OnMovePressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Move Pressed : %s"), *Value.ToString());
}

void ACropoutPlayer::OnSpinPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Spin Pressed : %s"), *Value.ToString());
}

void ACropoutPlayer::OnZoomPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom Pressed : %s"), *Value.ToString());
}
