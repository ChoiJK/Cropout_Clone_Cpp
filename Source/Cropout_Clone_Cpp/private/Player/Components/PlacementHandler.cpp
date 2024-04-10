// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PlacementHandler.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutPlayerController.h"
#include "Util/CoordinateUtils.h"
#include "Data/InteractableInfo.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "Player/CropoutPlayer.h"


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
			InputSubsystem->AddMappingContext(IMC_BuildMode, 0);

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

void UPlacementHandler::SetPlacementTargetEntity(AInteractableBaseActor* entity)
{
	PlacementTargetEntity = entity;
	PlacementTargetEntity->SetEnableGroundBlend(false);
	FVector IntersectionPos;
	CoordinateUtils::ProjectViewportCenterToGroundPlane(IntersectionPos);
	FTransform viewportCenterTransform = FTransform(FRotator(0, 0, 0),
		IntersectionPos);
	PlacementTargetEntity->SetActorTransform(viewportCenterTransform);
	PlacementTargetEntity->Tags = TArray<FName>{"PlacementMode"};


	PlacementSMC->AttachToComponent(PlacementTargetEntity->GetStaticMeshComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlacementSMC->SetVisibility(true);
}

void UPlacementHandler::ReleasePlacementTargetEntity()
{
	PlacementSMC->SetVisibility(false);
	PlacementSMC->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	if(IsValid(PlacementTargetEntity))
	{
		PlacementTargetEntity->Destroy();
		PlacementTargetEntity = nullptr;
	}
}

void UPlacementHandler::InitPlacementIMC()
{
	if(InputSubsystem)
	{
		InputSubsystem->AddMappingContext(IMC_BuildMode, 1);
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
	}
}

void UPlacementHandler::OnBuildMovePressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("UPlacementHandler - OnBuildMovePressed"));
}
