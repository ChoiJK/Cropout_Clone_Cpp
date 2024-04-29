// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/CropoutPlayer.h"

#include "limits"
#include "DrawDebugHelpers.h"

#include "Engine/EngineTypes.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Global/GlobalEventDispatcher.h"
#include "Player/Components/MovementInputHandler.h"
#include "Player/Components/VillagerHandler.h"
#include "Player/Components/PlacementHandler.h"
#include "Entity/Villager/Villager.h"
#include "Manager/TableManager.h"
#include "Spawn/Spawner.h"

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
	CursorMesh->SetGenerateOverlapEvents(false);
	CursorMesh->SetCollisionProfileName(TEXT("NoCollision"));

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
	Collision->SetSphereRadius(180.f);
	Collision->SetRelativeScale3D(FVector(0.666f, 0.666f, 1.f));
	Collision->SetCollisionObjectType(ECC_Pawn);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACropoutPlayer::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACropoutPlayer::OnEndOverlap);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MovementInputHandler = CreateDefaultSubobject<UMovementInputHandler>(TEXT("MovementInputHandler"));
	MovementInputHandler->Initialize(Movement);
	VillagerHandler = CreateDefaultSubobject<UVillagerHandler>(TEXT("VillagerHandler"));
	VillagerHandler->Initialize();
	PlacementHandler = CreateDefaultSubobject<UPlacementHandler>(TEXT("PlacementComponent"));
	PlacementHandler->Initialize();
}


void ACropoutPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FTimerManager& worldTimerManager = GetWorldTimerManager();
	if(worldTimerManager.IsTimerActive(ClosestHoverCheckTimerHandle) == false)
	{
		worldTimerManager.SetTimer(ClosestHoverCheckTimerHandle, this, &ACropoutPlayer::ClosestHoverCheck, 0.1f,
			true);
	}
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
		SetHoverActor(nullptr);
		GetWorldTimerManager().PauseTimer(ClosestHoverCheckTimerHandle);
	}
	else
	{
		AActor* NewHover = nullptr;
		bool villagerFound = false;
		float closestDistance = std::numeric_limits<float>::max();
		for(auto OverlappingActor : overlappingActors)
		{
			const bool isVillager = OverlappingActor->IsA<AVillager>();
			if(isVillager && villagerFound == false)
			{
				villagerFound = true;
				closestDistance = std::numeric_limits<float>::max();
				NewHover = nullptr;
			}

			if(villagerFound && isVillager == false)
			{
				continue;
			}

			const float newClosestDistance = FVector::DistSquared(OverlappingActor->GetActorLocation(),
				Collision->GetComponentLocation());
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
	RootComponent->SetWorldLocationAndRotation(FVector(0, 0, 0), FRotator(0, -45.f, 0));

	GameInstance = UCropoutGameInstance::GetInstance();
	if(GameInstance)
	{
		GlobalEventDispatcher = GameInstance->GetGlobalEventDispatcher();
		if(TSharedPtr<FGlobalEventDispatcher> EventDispatcher = GlobalEventDispatcher.Pin())
		{
			EventDispatcher->AddListenerUObject(EGlobalEventType::ChangedInputType, this,
				&ACropoutPlayer::OnChangedInputType);
		}
	}
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
	PlacementHandler->SetupPlayerInputComponent(PlayerInputComponent);
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

ASpawner* ACropoutPlayer::GetSpawner()
{
	if(Spawner == nullptr)
	{
		Spawner = Cast<ASpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner::StaticClass()));
		check(Spawner != nullptr);
	}

	return Spawner;
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

void ACropoutPlayer::BeginBuild(const FBuildableInfo* buildableInfo)
{
	UTableManager* tableManager = GameInstance->GetSystemManager<UTableManager>();
	const FInteractableInfo* interactableInfo = tableManager->GetInteractableInfo(buildableInfo->Name);

	if(interactableInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get interactable info %s "), *buildableInfo->Name.ToString());
		EndBuild();
	}

	if(PlacementHandler->CheckEntityAgainstPlacementTarget(interactableInfo) == false)
	{
		PlacementHandler->SetPlacementTargetEntity(interactableInfo);
	}
}

void ACropoutPlayer::EndBuild()
{
	PlacementHandler->ReleasePlacementTargetEntity();
}
