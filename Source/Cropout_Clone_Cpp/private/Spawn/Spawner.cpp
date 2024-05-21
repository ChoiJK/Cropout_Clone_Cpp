// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/Spawner.h"

#include "NavigationSystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Data/InteractableInfo.h"
#include "Manager/TableManager.h"
#include "Data/SpawnerInfo.h"
#include "Engine/StreamableManager.h"
#include "Entity/EntityManager.h"
#include "Entity/Interactable/Building/BuildingBaseActor.h"
#include "Entity/Interactable/Crop/CropBaseActor.h"
#include "Util/CoordinateUtils.h"


// Sets default values
ASpawner::ASpawner()
{
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = UCropoutGameInstance::GetInstance();
	TableManager = GameInstance->GetSystemManager<UTableManager>();
	SpawnerInfos = TableManager->GetSpawnerInfo();

	for(const FSpawnerInfo* spawnerInfo : SpawnerInfos)
	{
		if(spawnerInfo->SpawnEntityType == ESpawnEntityType::Interactable)
		{
			InteractableSpawnCount++;
		}
		else if(spawnerInfo->SpawnEntityType == ESpawnEntityType::Decoration)
		{
			DecorationSpawnCount++;
		}
	}

	if(AutoSpawn)
	{
		SpawnRandom();
	}
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(CheckNavMeshAndAsyncLoadTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void ASpawner::SpawnRandom()
{
	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, world]
		{
			Counter = 0;
			IndexCounter = 0;

			if(IsValid(world) == false)
			{
				return;
			}

			world->GetTimerManager().SetTimer(CheckNavMeshAndAsyncLoadTimerHandle,
				FTimerDelegate::CreateLambda(
					[this, world]
					{
						if(IsValid(this) == false)
						{
							return;
						}

						if(IsValid(world) == false)
						{
							return;
						}

						// @TODO : 존재하는 이슈 : Navigation에서의 Overlap이 정상적으로 진행되지 않음
						// 객체간 겹치는 이슈 있음.

						auto navigationSystem = world->GetNavigationSystem();
						auto worldSettings = GetWorldSettings();
						bool isNavigationBuilt = navigationSystem->IsNavigationBuilt(
							worldSettings);
						if(isNavigationBuilt == false)
						{
							return;
						}

						GetWorldTimerManager().PauseTimer(
							CheckNavMeshAndAsyncLoadTimerHandle);

						CheckNavMeshAndAsyncLoadFinished();
					}), 0.5f, true, -0.5);
		}));
	}
}

void ASpawner::SpawnInstanceFromSaveData(FName Name, FTransformsSaveData& TransformsSaveData)
{
	const FInteractableInfo* interactableInfo = TableManager->GetInteractableInfo(Name);
	if(interactableInfo->MeshList.Num() != 0)
	{
		FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
		UInstancedStaticMeshComponent* instancedSMC = Cast<UInstancedStaticMeshComponent>(
			AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, transform, false));

		UEntityManager* entityManager = GameInstance->GetLevelManager<UEntityManager>();
		if(entityManager != nullptr)
		{
			entityManager->AddDecoration(interactableInfo->Name, instancedSMC);
		}

		GameInstance->GetStreamableManager().RequestAsyncLoad(interactableInfo->MeshList[0].ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[interactableInfo, instancedSMC]()
				{
					instancedSMC->SetStaticMesh(
						interactableInfo->MeshList[0].Get());
				}));

		for(auto spawnTransform : TransformsSaveData.Transforms)
		{
			instancedSMC->AddInstance(spawnTransform, true);
		}
	}
}

void ASpawner::SpawnFromSaveData(const FSpawnEntitySaveData* SpawnInstance)
{
	const FInteractableInfo* interactableInfo = TableManager->GetInteractableInfo(SpawnInstance->Name);
	if(interactableInfo == nullptr)
	{
		return;
	}

	FTransform spawnTransform = FTransform(SpawnInstance->Rotation,
		SpawnInstance->Location,
		SpawnInstance->Scale);

	AInteractableBaseActor* spawnedActor = SpawnInteractable(interactableInfo, SpawnInstance);
	if(spawnedActor == nullptr)
	{
		return;
	}
	spawnedActor->SetActorTransform(spawnTransform);

	if(spawnedActor->IsA(AResourceBaseActor::StaticClass()))
	{
		AResourceBaseActor* resourceActor = Cast<AResourceBaseActor>(spawnedActor);
		resourceActor->DoScaleUp(1);
	}
}


AInteractableBaseActor* ASpawner::SpawnInteractable(const FInteractableInfo* InteractableInfo,
	const FSpawnEntitySaveData* LoadData)
{
	if(InteractableInfo == nullptr)
	{
		return nullptr;
	}

	AInteractableBaseActor* spawnedActor = nullptr;
	if(InteractableInfo->InteractableType == EInteractableType::Resource)
	{
		spawnedActor = Cast<AInteractableBaseActor>(
			GetWorld()->SpawnActor(AResourceBaseActor::StaticClass()));
	}
	else if(InteractableInfo->InteractableType == EInteractableType::Building)
	{
		spawnedActor = Cast<AInteractableBaseActor>(
			GetWorld()->SpawnActor(ABuildingBaseActor::StaticClass()));
	}
	else if(InteractableInfo->InteractableType == EInteractableType::Field)
	{
		spawnedActor = Cast<AInteractableBaseActor>(
			GetWorld()->SpawnActor(ACropBaseActor::StaticClass()));
	}

	if(spawnedActor != nullptr)
	{
		spawnedActor->SetInteractableInfo(InteractableInfo, LoadData);
	}

	return spawnedActor;
}

void ASpawner::ClearSpawnerTimer()
{
	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(CheckNavMeshAndAsyncLoadTimerHandle);
	}
}

void ASpawner::CheckNavMeshAndAsyncLoadFinished()
{
	const FSpawnerInfo* spawnerInfo = SpawnerInfos[IndexCounter];
	const FInteractableInfo* interactableInfo = TableManager->GetInteractableInfo(spawnerInfo->Name);

	if(spawnerInfo != nullptr && interactableInfo != nullptr)
	{
		if(spawnerInfo->SpawnEntityType == ESpawnEntityType::Interactable)
		{
			// Spawn of Class
			UE_LOG(LogTemp, Warning, TEXT("Spawn of Class"));

			SpawnAssets(spawnerInfo, interactableInfo);
		}
		else if(spawnerInfo->SpawnEntityType == ESpawnEntityType::Decoration)
		{
			// Spawn Instances
			UE_LOG(LogTemp, Warning, TEXT("Spawn Instances"));

			if(interactableInfo->MeshList.Num() != 0)
			{
				FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
				UInstancedStaticMeshComponent* instancedSMC = Cast<UInstancedStaticMeshComponent>(
					AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, transform, false));

				UEntityManager* entityManager = GameInstance->GetLevelManager<UEntityManager>();
				if(entityManager != nullptr)
				{
					entityManager->AddDecoration(interactableInfo->Name, instancedSMC);
				}

				GameInstance->GetStreamableManager().RequestAsyncLoad(interactableInfo->MeshList[0].ToSoftObjectPath(),
					FStreamableDelegate::CreateLambda(
						[interactableInfo, instancedSMC]()
						{
							instancedSMC->SetStaticMesh(
								interactableInfo->MeshList[0].Get());
						}));

				SpawnInstance(instancedSMC, spawnerInfo->BiomeScale, spawnerInfo->BiomeCount,
					spawnerInfo->SpawnPerBiome);
			}
		}
	}

	IndexCounter++;

	// 하나씩 스폰하고, 다시 IsNavigationBuilt를 async로 체크한다. 이렇게 안하면 무슨일이 발생하지..?
	if(SpawnerInfos.Num() == IndexCounter)
	{
		IndexCounter = 0;
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(
			CheckNavMeshAndAsyncLoadTimerHandle);
	}
}


void ASpawner::SpawnAssets(const FSpawnerInfo* SpawnData, const FInteractableInfo* InteractableInfo)
{
	UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if(NavSys == nullptr)
	{
		return;
	}

	if(NavData == nullptr)
	{
		return;
	}

	for(int i = 0; i < SpawnData->BiomeCount; ++i)
	{
		// GetRandomLocationInNavigableRadius
		FNavLocation RandomPoint;

		if(NavSys->GetRandomPointInNavigableRadius(FVector::Zero(), 10000, RandomPoint, NavData, nullptr))
		{
			FVector randomLocation = RandomPoint.Location;

			int targetIndex = Seed.RandRange(0, SpawnData->SpawnPerBiome);
			for(int j = 0; j < targetIndex; ++j)
			{
				if(NavSys->GetRandomPointInNavigableRadius(randomLocation, SpawnData->BiomeScale, RandomPoint,
					NavData, nullptr))
				{
					FVector spawnPos = CoordinateUtils::SteppedPosition(RandomPoint.Location);
					float rotYaw = FMath::FRandRange(0, SpawnData->RandomRotationRange);
					float scale = FMath::FRandRange(1, 1 + SpawnData->ScaleRange);

					FTransform spawnTransform = FTransform(FRotator(0, rotYaw, 0),
						spawnPos,
						FVector(scale, scale, scale));

					AResourceBaseActor* spawnedActor = Cast<AResourceBaseActor>(SpawnInteractable(InteractableInfo));
					spawnedActor->SetActorTransform(spawnTransform);
					spawnedActor->DoScaleUp(Counter / TotalCount);
					Counter++;
				}
			}
		}
	}
}

void ASpawner::SpawnInstance(UInstancedStaticMeshComponent* iSMC, const float radius, const int biomeCount,
	const int maxSpawnCount)
{
	UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if(NavSys == nullptr)
	{
		return;
	}

	if(NavData == nullptr)
	{
		return;
	}

	for(int i = 0; i < biomeCount; ++i)
	{
		// GetRandomLocationInNavigableRadius
		FNavLocation RandomPoint;

		if(NavSys->GetRandomPointInNavigableRadius(FVector::Zero(), 10000, RandomPoint, NavData, nullptr))
		{
			FVector randomLocation = RandomPoint.Location;

			int targetIndex = Seed.RandRange(0, maxSpawnCount);
			for(int j = 0; j < targetIndex; ++j)
			{
				if(NavSys->GetRandomPointInNavigableRadius(randomLocation, radius, RandomPoint,
					NavData, nullptr))
				{
					FVector spawnPos = RandomPoint.Location;

					float distance = (randomLocation - spawnPos).Length();
					float scale = FMath::Lerp(0.8, 1.5, distance / radius);
					spawnPos.Z = 0.f;
					FTransform spawnTransform = FTransform(FRotator(0, 0, 0),
						spawnPos,
						FVector(scale, scale, scale));

					iSMC->AddInstance(spawnTransform, true);
					Counter++;
				}
			}
		}
	}
}
