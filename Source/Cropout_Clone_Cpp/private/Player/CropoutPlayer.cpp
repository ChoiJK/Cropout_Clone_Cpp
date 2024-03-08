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
#include "Player/Components/VillagerHandler.h"

#include "limits"
#include "DrawDebugHelpers.h"

ACropoutPlayer::ACropoutPlayer()
{
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
	CursorMesh->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	CursorMesh->SetRelativeScale3D(FVector(2.f, 2.f, 1.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMplane
		(TEXT("Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	check(SMplane.Succeeded());
	if(SMplane.Succeeded())
	{
		CursorMesh->SetStaticMesh(SMplane.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MIcursor
		(TEXT("Engine.MaterialInstanceConstant'/Game/VFX/Materials/MI_Cursor.MI_Cursor'"));
	check(MIcursor.Succeeded());
	if(MIcursor.Succeeded())
	{
		CursorMesh->SetMaterial(0, MIcursor.Object);
	}

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(GetRootComponent());
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACropoutPlayer::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACropoutPlayer::OnEndOverlap);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MovementInputHandler = CreateDefaultSubobject<UMovementInputHandler>(TEXT("MovementInputHandler"));
	MovementInputHandler->Initialize(Movement);
	VillagerHandler = CreateDefaultSubobject<UVillagerHandler>(TEXT("VillagerHandler"));
	VillagerHandler->Initialize();
}


void ACropoutPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if(HoverActor->IsValidLowLevel())
	{
		return;
	}

	if(OtherActor == this)
	{
		return;
	}

	SetHoverActor(OtherActor);

	GetWorldTimerManager().SetTimer(ClosestHoverCheckTimerHandle, this, &ACropoutPlayer::ClosestHoverCheck, 0.01f,
	                                true);
}

void ACropoutPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> overlappingActors;
	Collision->GetOverlappingActors(overlappingActors, AActor::StaticClass());
	if(overlappingActors.IsEmpty())
	{
		GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			SetHoverActor(nullptr);
		}));
	}
}

void ACropoutPlayer::ClosestHoverCheck()
{
	TArray<AActor*> overlappingActors;
	Collision->GetOverlappingActors(overlappingActors, AActor::StaticClass());
	if(overlappingActors.IsEmpty())
	{
		GetWorldTimerManager().PauseTimer(ClosestHoverCheckTimerHandle);
	}
	else
	{
		AActor* NewHover = nullptr;
		float closestDistance = std::numeric_limits<float>::max();
		for(auto OverlappingActor : overlappingActors)
		{
			float newClosestDistance = FVector::DistSquared(GetActorLocation(), Collision->GetComponentLocation());
			if(NewHover != this && closestDistance > newClosestDistance)
			{
				closestDistance = newClosestDistance;
				NewHover = OverlappingActor;
			}
		}

		if(NewHover != HoverActor)
		{
			SetHoverActor(NewHover);
		}
	}
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

	MovementInputHandler->UpdateZoom();

	FTimerHandle MoveTrackingHandle;
	GetWorld()->GetTimerManager().SetTimer(MoveTrackingHandle, MovementInputHandler,
	                                       &UMovementInputHandler::MoveTracking, 0.016666f,
	                                       true, 0.0f);
}

void ACropoutPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), Collision->GetComponentLocation(), Collision->GetUnscaledSphereRadius(), 20,
	                FColor::Red);
}

// Called to bind functionality to input
void ACropoutPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	MovementInputHandler->SetupPlayerInputComponent(PlayerInputComponent);
	VillagerHandler->SetupPlayerInputComponent(PlayerInputComponent);
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
