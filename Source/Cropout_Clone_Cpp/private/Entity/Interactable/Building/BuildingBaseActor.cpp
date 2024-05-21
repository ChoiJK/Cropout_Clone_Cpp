// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Interactable/Building/BuildingBaseActor.h"

#include "Components/BoxComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Core/InGame/InGameGameMode.h"
#include "Core/ApplicationState/ApplicationStateManager.h"
#include "Data/InteractableInfo.h"
#include "Entity/EntityManager.h"
#include "Spawn/Spawner.h"

ABuildingBaseActor::ABuildingBaseActor()
{
	// @Todo : NavBloker 적용되지않음
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

void ABuildingBaseActor::SetInteractableInfo(const FInteractableInfo* InInfo, const FSpawnEntitySaveData* LoadData)
{
	Super::SetInteractableInfo(InInfo, LoadData);

	RemainingBuildTimeSec = LoadData ? LoadData->FloatValue : BuildTimeSec;
	BuildingType = InInfo->BuildingType;
	NewVillagerCount = InInfo->NewVillagerCount;
	if(RequireBuild)
	{
		Tags.Add("Build");
	}
	else
	{
		Tags.Add("Building");
	}
}

FSpawnEntitySaveData ABuildingBaseActor::GetSaveData()
{
	FSpawnEntitySaveData buildingData;
	InternalGetSaveData(buildingData);
	buildingData.FloatValue = RemainingBuildTimeSec;
	return buildingData;
}

void ABuildingBaseActor::BuildTick()
{
	float deltaSec = 0.2f;
	if(RequireBuild == false)
	{
		return;
	}

	if(RemainingBuildTimeSec > 0.0f)
	{
		float buildMultiplier = Builders.Num();

		RemainingBuildTimeSec -= (deltaSec * buildMultiplier);
		BuildProgress = 1.0f - (RemainingBuildTimeSec / BuildTimeSec);
	}
	else
	{
		RemainingBuildTimeSec = 0.0f;
		BuildProgress = 1.0f;
		RequireBuild = false;

		BuildSuccess();
	}

	ChangeMesh(BuildProgress * (MeshList.Num() - 1));
}

float ABuildingBaseActor::Interact()
{
	return RemainingBuildTimeSec;
}

bool ABuildingBaseActor::IsRequireBuild() const
{
	return RequireBuild;
}

void ABuildingBaseActor::AddBuilder(AVillager* Builder)
{
	if(Builder == nullptr)
	{
		return;
	}

	if(Builders.Contains(Builder) == false)
	{
		Builders.Add(Builder);
	}

	if(Builders.Num() > 0)
	{
		if(BuildTimerHandle.IsValid() == false)
		{
			GetWorldTimerManager().SetTimer(BuildTimerHandle,
				FTimerDelegate::CreateWeakLambda(this, [this]()
				{
					BuildTick();
				}), 0.2f, true, 0.2f);
		}
	}
}

void ABuildingBaseActor::RemoveBuilder(AVillager* Builder)
{
	if(Builder == nullptr)
	{
		return;
	}

	if(Builders.Contains(Builder))
	{
		Builders.Remove(Builder);
	}

	if(Builders.Num() == 0)
	{
		if(BuildTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(BuildTimerHandle);
		}
	}
}


// Called when the game starts or when spawned
void ABuildingBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildingBaseActor::InitializeStaticMesh(int index)
{
	Super::InitializeStaticMesh();

	if(MeshList.Num() != 0)
	{
		ChangeMesh(index == -1 ? 0 : index);
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


void ABuildingBaseActor::BuildSuccess()
{
	Tags.Remove("Build");
	Tags.Add("Building");

	if(EntityManager != nullptr)
	{
		EntityManager->RemoveBuilding(this);
	}

	switch(BuildingType)
	{
	case EBuildingType::House:
		{
			UCropoutGameInstance* GameInstance = UCropoutGameInstance::GetInstance();
			UEntityManager* entityManager = GameInstance->GetLevelManager<UEntityManager>();
			for(int i = 0; i < NewVillagerCount; i++)
			{
				entityManager->SpawnVillager();
			}
			break;
		}

	case EBuildingType::VictoryMonument:
		{
			UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
			UApplicationStateManager* ASM = gameInstance->GetSystemManager<UApplicationStateManager>();
			ASM->TriggerStateTransition(EStateTrigger::WinGame);
			break;
		}
	default: ;
	}
}
