// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/PlacementHandler.h"

#include "Core/CropoutGameInstance.h"
#include "Util/CoordinateUtils.h"
#include "Data/InteractableInfo.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "Player/CropoutPlayer.h"


UPlacementHandler::UPlacementHandler()
{
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
}

void UPlacementHandler::ReleasePlacementIMC()
{
}
