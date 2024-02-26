// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/Spawner.h"

#include "NavigationSystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Engine/StreamableManager.h"


// Sets default values
ASpawner::ASpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UCropoutGameInstance>(GetGameInstance());

	AsyncLoadClasses();

	if(AutoSpawn)
	{
		SpawnRandom();
	}
}

void ASpawner::AsyncLoadClasses()
{
	ClassRefIndex = 0;
	AsyncLoadComplete = false;

	AsyncLoad();
}

void ASpawner::AsyncLoad()
{
	TArray<FSoftObjectPath> classRefs;
	for(auto& spawnType : SpawnTypes)
	{
		classRefs.Add(spawnType.ClassRef.ToSoftObjectPath());
	}

	auto handle = GameInstance->GetStreamableManager().RequestAsyncLoad(classRefs, FStreamableDelegate::CreateUObject(
		                                                                    this, &ASpawner::AsyncLoadDeferred));
}

void ASpawner::AsyncLoadDeferred()
{
	AsyncLoadComplete = true;
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawner::SpawnRandom()
{
	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, world]
		{
			Counter = 0;
			IndexCounter = 0;

			world->GetTimerManager().SetTimer(CheckNavMeshAndAsyncLoadTimerHandle, FTimerDelegate::CreateLambda(
				                                  [this, world]
				                                  {
					                                  if(AsyncLoadComplete == false)
					                                  {
						                                  return;
					                                  }

					                                  auto navigationSystem = world->GetNavigationSystem();
					                                  if(navigationSystem == nullptr)
					                                  {
						                                  return;
					                                  }

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

void ASpawner::CheckNavMeshAndAsyncLoadFinished()
{
	if(ActorSwitch)
	{
		// Spawn of Class
		UE_LOG(LogTemp, Warning, TEXT("Spawn of Class"));

		// 하나씩 스폰하고, 다시 IsNavigationBuilt를 async로 체크한다. 이렇게 안하면 무슨일이 발생하지..?
		SpawnAssets(SpawnTypes[IndexCounter++]);

		if(IndexCounter == SpawnTypes.Num())
		{
			IndexCounter = 0;
			ActorSwitch = false;
		}

		GetWorldTimerManager().UnPauseTimer(
			CheckNavMeshAndAsyncLoadTimerHandle);
	}
	else
	{
		// Spawn Instances
		UE_LOG(LogTemp, Warning, TEXT("Spawn Instances"));

		FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0));
		UInstancedStaticMeshComponent* instancedSMC = Cast<UInstancedStaticMeshComponent>(
			AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, transform, false));
		FSpawnInstance spawnInstance = SpawnInstances[IndexCounter++];
		instancedSMC->SetStaticMesh(spawnInstance.ClassRef);

		SpawnInstance(instancedSMC, spawnInstance.BiomeScale, spawnInstance.BiomeCount, spawnInstance.SpawnPerBiome);

		if(IndexCounter == SpawnTypes.Num())
		{
			IndexCounter = 0;
			ActorSwitch = true;
		}
		else
		{
			GetWorldTimerManager().UnPauseTimer(
				CheckNavMeshAndAsyncLoadTimerHandle);
		}
	}
}


void ASpawner::SpawnAssets(const FSpawnData& SpawnData)
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

	UClass* ActorClass = SpawnData.ClassRef.LoadSynchronous();
	if(ActorClass == nullptr)
	{
		return;
	}

	for(int i = 0; i < SpawnData.BiomeCount; ++i)
	{
		// GetRandomLocationInNavigableRadius
		FNavLocation RandomPoint;

		if(NavSys->GetRandomPointInNavigableRadius(FVector::Zero(), 10000, RandomPoint, NavData, nullptr))
		{
			FVector randomLocation = RandomPoint.Location;

			int targetIndex = Seed.RandRange(0, SpawnData.SpawnPerBiome);
			for(int j = 0; j < targetIndex; ++j)
			{
				if(NavSys->GetRandomPointInNavigableRadius(randomLocation, SpawnData.BiomeScale, RandomPoint,
				                                           NavData, nullptr))
				{
					FVector spawnPos = RandomPoint.Location;
					float rotYaw = FMath::FRandRange(0, SpawnData.RandomRotationRange);
					float scale = FMath::FRandRange(1, 1 + SpawnData.ScaleRange);

					FTransform spawnTransform = FTransform(FRotator(0, rotYaw, 0),
					                                       spawnPos,
					                                       FVector(scale, scale, scale));

					GetWorld()->SpawnActor(ActorClass, &spawnTransform);
					Counter++;

					if(const auto sharedVariable = GameInstance->GetGlobalSharedVariable().Pin())
					{
						sharedVariable->SetScaleUpDelay(Counter / TotalCount);
					}
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

					if(const auto sharedVariable = GameInstance->GetGlobalSharedVariable().Pin())
					{
						sharedVariable->SetScaleUpDelay(Counter / TotalCount);
					}
				}
			}
		}
	}
}
