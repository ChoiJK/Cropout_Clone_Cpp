// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Building/BuildingBaseActor.h"

#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"

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
	if(MeshList.Num() != 0)
	{
		ChangeMesh(MeshList.Num() - 1);
	}
}


// Called when the game starts or when spawned
void ABuildingBaseActor::BeginPlay()
{
	Super::BeginPlay();
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
