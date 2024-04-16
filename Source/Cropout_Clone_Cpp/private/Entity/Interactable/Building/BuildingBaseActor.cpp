// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Interactable/Building/BuildingBaseActor.h"

#include "Components/BoxComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Data/InteractableInfo.h"
#include "Entity/EntityManager.h"

ABuildingBaseActor::ABuildingBaseActor()
{
	// @Todo : NavBloker 적용되지않음
	PrimaryActorTick.bCanEverTick = true;
	NavBlocker = CreateDefaultSubobject<UBoxComponent>(TEXT("NavBloker"));
	NavBlocker->SetupAttachment(StaticMeshComponent);

	NavBlocker->PrimaryComponentTick.bCanEverTick = false;

	NavBlocker->SetMobility(EComponentMobility::Movable);
	NavBlocker->SetShouldUpdatePhysicsVolume(true);
	NavBlocker->SetCanEverAffectNavigation(true);

	NavBlocker->bDynamicObstacle = true;
}


void ABuildingBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ABuildingBaseActor::BeginDestroy()
{
	Super::BeginDestroy();

	if(EntityManager != nullptr)
	{
		EntityManager->RemoveBuilding(this);
	}
}

void ABuildingBaseActor::SetInteractableInfo(const FInteractableInfo* InInfo)
{
	Super::SetInteractableInfo(InInfo);

	if(RequireBuild)
	{
		Tags.Add("Build");
	}
	else
	{
		Tags.Add("Building");
	}
}


// Called when the game starts or when spawned
void ABuildingBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildingBaseActor::InitializeStaticMesh()
{
	Super::InitializeStaticMesh();

	if(MeshList.Num() != 0)
	{
		ChangeMesh(0);
	}
}

void ABuildingBaseActor::RegisterWithEntityManager()
{
	if(EntityManager != nullptr)
	{
		EntityManager->AddBuilding(this, RequireBuild);
	}
}

void ABuildingBaseActor::ChangeMesh(int index)
{
	if(index >= 0 && index < MeshList.Num())
	{
		if(StaticMeshComponent->GetStaticMesh() != MeshList[index])
		{
			StaticMeshComponent->SetStaticMesh(MeshList[index]);
		}
		NavBlocker->SetBoxExtent(StaticMeshComponent->Bounds.BoxExtent);
		NavBlocker->SetWorldLocation(StaticMeshComponent->Bounds.Origin);
	}
}
