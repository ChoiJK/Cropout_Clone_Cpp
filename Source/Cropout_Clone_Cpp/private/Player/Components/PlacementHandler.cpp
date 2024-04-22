// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PlacementHandler.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GroomVisualizationData.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Camera/CameraActor.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Util/CoordinateUtils.h"
#include "Data/InteractableInfo.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "Materials/MaterialParameterCollection.h"
#include "Player/CropoutPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Spawn/Spawner.h"


UPlacementHandler::UPlacementHandler()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imc_BuildMode
		(TEXT("EnhancedInput.InputMappingContext'/Game/Blueprint/Player/Input/IMC_BuildMode.IMC_BuildMode'"));
	if(imc_BuildMode.Succeeded())
	{
		IMC_BuildMode = imc_BuildMode.Object;
	}
	check(IMC_BuildMode);

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_BuildMove
		(TEXT("EnhancedInput.InputAction'/Game/Blueprint/Player/Input/IA_BuildMove.IA_BuildMove'"));
	if(ia_BuildMove.Succeeded())
	{
		IA_BuildMove = ia_BuildMove.Object;
	}
	check(IA_BuildMove);
}

void UPlacementHandler::BeginPlay()
{
	Super::BeginPlay();
	GlobalSharedVariable = UCropoutGameInstance::GetInstance()->GetGlobalSharedVariable();

	PlacementSMC->SetStaticMesh(GlobalSharedVariable->GetPrimitiveShapeMesh(EPrimitiveShapeType::Cube));
	PlacementSMC->SetMaterial(0, GlobalSharedVariable->GetPlaceableMaterial());
}

void UPlacementHandler::Initialize()
{
	Owner = Cast<ACropoutPlayer>(GetOwner());

	PlacementSMC = Owner->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlacementMesh"));
	PlacementSMC->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PlacementSMC->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	PlacementSMC->SetGenerateOverlapEvents(false);
	PlacementSMC->SetCollisionProfileName(TEXT("NoCollision"));
	PlacementSMC->SetVisibility(false);
}

void UPlacementHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(const ULocalPlayer* LocalPlayer = Owner->GetPlayerController()->GetLocalPlayer())
	{
		InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if(InputSubsystem)
		{
			if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
			{
				EnhancedInputComponent->BindAction(IA_BuildMove, ETriggerEvent::Triggered, this,
					&UPlacementHandler::OnBuildMovePressed);
			}
		}
	}
}

bool UPlacementHandler::CheckEntityAgainstPlacementTarget(const FInteractableInfo* info)
{
	if(IsValid(PlacementTargetEntity) == false)
	{
		return false;
	}

	if(PlacementTargetEntity->GetName() == info->Name)
	{
		return true;
	}

	PlacementTargetEntity->Destroy();
	PlacementTargetEntity = nullptr;

	return false;
}

void UPlacementHandler::SetPlacementTargetEntity(const FInteractableInfo* interactableInfo)
{
	PlacementTargetInfo = interactableInfo;

	AInteractableBaseActor* interactableActor = Owner->GetSpawner()->SpawnInteractable(PlacementTargetInfo);
	if(interactableActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn interactable actor"));
		Owner->EndBuild();
		return;
	}

	PlacementTargetEntity = interactableActor;
	PlacementTargetEntity->SetEnableGroundBlend(false);
	FVector IntersectionPos;
	CoordinateUtils::ProjectViewportCenterToGroundPlane(IntersectionPos);
	FTransform viewportCenterTransform = FTransform(FRotator(0, 45.f, 0),
		IntersectionPos);
	PlacementTargetEntity->SetActorTransform(viewportCenterTransform);
	PlacementTargetEntity->Tags = TArray<FName>{"PlacementMode"};
	PlacementTargetRecalcBoxExtentDelegateHandle = PlacementTargetEntity->AddReCalcBoxExtentDelegateHandle(this,
		&UPlacementHandler::PostPlacementTargetRecalcBoxExtent);
}

void UPlacementHandler::PostPlacementTargetRecalcBoxExtent()
{
	FVector boundOrigin;
	FVector boundExtent;
	PlacementTargetEntity->GetActorBounds(false, boundOrigin, boundExtent);

	PlacementSMC->AttachToComponent(PlacementTargetEntity->GetStaticMeshComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlacementSMC->SetRelativeScale3D(boundExtent / 40.0f);
	PlacementSMC->SetVisibility(true);

	UpdateBuildAsset();
}

void UPlacementHandler::ReleasePlacementTargetEntity()
{
	PlacementSMC->SetVisibility(false);
	PlacementSMC->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	if(IsValid(PlacementTargetEntity))
	{
		PlacementTargetEntity->RemoveReCalcBoxExtentDelegateHandle(PlacementTargetRecalcBoxExtentDelegateHandle);

		PlacementTargetEntity->Destroy();
		PlacementTargetEntity = nullptr;
	}
	PlacementTargetInfo = nullptr;
}

void UPlacementHandler::PlacingEntity()
{
	if(IsValid(PlacementTargetEntity) == false)
	{
		return;
	}

	AInteractableBaseActor* interactableActor = Owner->GetSpawner()->SpawnInteractable(PlacementTargetInfo);
	if(interactableActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn interactable actor"));
		Owner->EndBuild();
		return;
	}

	interactableActor->SetActorTransform(PlacementTargetEntity->GetActorTransform());

	if(PlacementTargetInfo->InteractableType == EInteractableType::Resource)
	{
		AResourceBaseActor* resourceActor = Cast<AResourceBaseActor>(interactableActor);
		resourceActor->DoScaleUp(0.1f);
	}

	UpdateBuildAsset();
}

void UPlacementHandler::InitPlacementIMC()
{
	if(InputSubsystem)
	{
		InputSubsystem->AddMappingContext(IMC_BuildMode, 0);
		HasMappingContext = true;
	}
}

void UPlacementHandler::ReleasePlacementIMC()
{
	if(InputSubsystem)
	{
		FModifyContextOptions options;
		options.bIgnoreAllPressedKeysUntilRelease = true;
		options.bForceImmediately = true;
		options.bNotifyUserSettings = false;

		InputSubsystem->RemoveMappingContext(IMC_BuildMode, options);
		HasMappingContext = false;
	}
}

FVector2D UPlacementHandler::GetPlacementTargetBottomScreenPosition() const
{
	FVector2D screenCenter = CoordinateUtils::GetViewportCenter();
	if(PlacementTargetEntity == nullptr)
	{
		return screenCenter;
	}

	const ACropoutPlayerController* playerController = Owner->GetPlayerController();
	FVector2D PlacementTargetBottomScreenPosition;

	FVector boundOrigin;
	FVector boundExtent;
	PlacementTargetEntity->GetActorBounds(false, boundOrigin, boundExtent);
	FVector worldPosition = PlacementTargetEntity->GetActorLocation();

	FVector2D downScreen = screenCenter + FVector2D(0, 50);

	FVector centerLocation, downLocation;
	CoordinateUtils::ProjectScreenPosToGroundPlane(screenCenter, centerLocation);
	CoordinateUtils::ProjectScreenPosToGroundPlane(downScreen, downLocation);

	FVector downDirection = downLocation - centerLocation;
	downDirection.Normalize();

	worldPosition += downDirection * FMathf::Min(boundExtent.X, boundExtent.Y) * 1.2f;

	playerController->ProjectWorldLocationToScreen(worldPosition,
		PlacementTargetBottomScreenPosition, true);

	return PlacementTargetBottomScreenPosition;
}

void UPlacementHandler::UpdateBuildAsset()
{
	TArray<AActor*> overlappingActors;
	PlacementTargetEntity->GetOverlappingActors(overlappingActors, AInteractableBaseActor::StaticClass());

	bool CanDrop = false;
	if(overlappingActors.Num() == 0)
	{
		if(ConersInNav())
		{
			CanDrop = true;
		}
	}

	FVector targetPosition = PlacementTargetEntity->GetActorLocation();
	FLinearColor targetPositionColor = FLinearColor(targetPosition);
	targetPositionColor.A = CanDrop ? 1.0f : 0.0f;

	UMaterialParameterCollection* cropoutMPC = GlobalSharedVariable->GetCropoutMPC();

	if(cropoutMPC == nullptr)
	{
		return;
	}

	UMaterialParameterCollectionInstance* inst = GetWorld()->GetParameterCollectionInstance(cropoutMPC);

	FLinearColor oldTargetPosition;
	if(inst->GetVectorParameterValue("Target Position", oldTargetPosition))
	{
		inst->SetVectorParameterValue("Target Position", targetPositionColor);
	}
}

bool UPlacementHandler::ConersInNav()
{
	FVector boundOrigin;
	FVector boundExtent;
	PlacementTargetEntity->GetActorBounds(false, boundOrigin, boundExtent);

	const float ExtentX = boundExtent.X * 1.05f;
	const float ExtentY = boundExtent.Y * 1.05f;
	const float ExtentMinusX = ExtentX - 1.0f;
	const float ExtentMinusY = ExtentY - 1.0f;

	FVector corner1 = boundOrigin + FVector(ExtentMinusX, ExtentMinusY, 0);
	FVector corner1_up = FVector(corner1.X, corner1.Y, 100);
	FVector corner1_down = FVector(corner1.X, corner1.Y, -1);

	FVector corner2 = boundOrigin + FVector(ExtentMinusX, -ExtentMinusY, 0);
	FVector corner2_up = FVector(corner2.X, corner2.Y, 100);
	FVector corner2_down = FVector(corner2.X, corner2.Y, -1);

	FVector corner3 = boundOrigin + FVector(-ExtentMinusX, ExtentMinusY, 0);
	FVector corner3_up = FVector(corner3.X, corner3.Y, 100);
	FVector corner3_down = FVector(corner3.X, corner3.Y, -1);

	FVector corner4 = boundOrigin + FVector(-ExtentMinusX, -ExtentMinusY, 0);
	FVector corner4_up = FVector(corner4.X, corner4.Y, 100);
	FVector corner4_down = FVector(corner4.X, corner4.Y, -1);

	TArray<FHitResult> hitResults;

	if(UKismetSystemLibrary::LineTraceMulti(GetWorld(), corner1_up, corner1_down,
		static_cast<ETraceTypeQuery>(static_cast<int>(ECC_Visibility)), false, TArray<AActor*>(), EDrawDebugTrace::None,
		hitResults, true) == false)
	{
		return false;
	}

	if(UKismetSystemLibrary::LineTraceMulti(GetWorld(), corner2_up, corner2_down,
		static_cast<ETraceTypeQuery>(static_cast<int>(ECC_Visibility)), false, TArray<AActor*>(), EDrawDebugTrace::None,
		hitResults, true) == false)
	{
		return false;
	}

	if(UKismetSystemLibrary::LineTraceMulti(GetWorld(), corner3_up, corner3_down,
		static_cast<ETraceTypeQuery>(static_cast<int>(ECC_Visibility)), false, TArray<AActor*>(), EDrawDebugTrace::None,
		hitResults, true) == false)
	{
		return false;
	}

	if(UKismetSystemLibrary::LineTraceMulti(GetWorld(), corner4_up, corner4_down,
		static_cast<ETraceTypeQuery>(static_cast<int>(ECC_Visibility)), false, TArray<AActor*>(), EDrawDebugTrace::None,
		hitResults, true) == false)
	{
		return false;
	}

	return true;
}

void UPlacementHandler::OnBuildMovePressed(const FInputActionValue& Value)
{
	if(CoordinateUtils::SingleTouchCheck())
	{
		TrackMovePlacement();
	}
	else
	{
		ReleasePlacementIMC();
	}
}

void UPlacementHandler::UpdateTrackMovePlacement()
{
	if(HasMappingContext)
	{
		TrackMovePlacement();
	}
}

void UPlacementHandler::RotatePlacementEntity()
{
	PlacementTargetEntity->AddActorWorldRotation(FRotator(0, 90, 0));
}

void UPlacementHandler::TrackMovePlacement()
{
	FVector2D ScreenPos;
	FVector IntersectionPos;
	if(CoordinateUtils::ProjectTouchToGroundPlane(ScreenPos, IntersectionPos))
	{
		PlacementTargetEntity->SetActorLocation(CoordinateUtils::SteppedPosition(IntersectionPos));
	}

	UpdateBuildAsset();
}
