// Fill out your copyright notice in the Description page of Project Settings.

#include "Entity/Interactable/Crop/CropBaseActor.h"
#include "Components/TimelineComponent.h"
#include "Data/InteractableInfo.h"
#include "Entity/EntityManager.h"
#include "Enum/ResourceType.h"

ACropBaseActor::ACropBaseActor()
{
	if(IsInitResourceScaleCurveSuccess() == false)
	{
		Destroy();
		return;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ACropBaseActor::SetInteractableInfo(const FInteractableInfo* InInfo, const FSpawnEntitySaveData* LoadData)
{
	Super::SetInteractableInfo(InInfo, LoadData);

	GrowthTime = InInfo->GrowthTime;
	HarvestAmount = InInfo->HarvestAmount;
	CurrentGrowthTime = LoadData ? LoadData->FloatValue : 0.f;
	GrowthProgress = FMath::Clamp(CurrentGrowthTime / GrowthTime, 0.f, 1.f);
	ChangeMesh(GrowthProgress * (MeshList.Num() - 1));

	Tags.Add("Farming");
}

FSpawnEntitySaveData ACropBaseActor::GetSaveData()
{
	FSpawnEntitySaveData buildingData;
	InternalGetSaveData(buildingData);
	buildingData.FloatValue = CurrentGrowthTime;
	return buildingData;
}

void ACropBaseActor::BeginDestroy()
{
	Super::BeginDestroy();

	if(EntityManager != nullptr)
	{
		EntityManager->RemoveCrop(this);
	}
}

void ACropBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACropBaseActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));
	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("TimelineFinished"));

	Timeline.AddInterpFloat(Curve, TimelineProgress);
	Timeline.SetTimelineFinishedFunc(TimelineFinished);
}

void ACropBaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACropBaseActor::InitializeStaticMesh(int index)
{
	Super::InitializeStaticMesh();

	if(MeshList.Num() != 0)
	{
		ChangeMesh(index == -1 ? 0 : index, true);
	}
}

void ACropBaseActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Timeline.IsPlaying())
	{
		Timeline.TickTimeline(DeltaSeconds);
	}

	if(IsGrowthProcess)
	{
		if(CurrentGrowthEndTime > CurrentGrowthTime)
		{
			CurrentGrowthTime += DeltaSeconds;
			GrowthProgress = FMath::Clamp(CurrentGrowthTime / GrowthTime, 0.f, 1.f);
		}
		else
		{
			IsGrowthProcess = false;

			if(IsReadyToHarvest())
			{
				EntityManager->ChangeCropState(this, true, false);
			}
		}

		ChangeMesh(GrowthProgress * (MeshList.Num() - 1));
	}
}

bool ACropBaseActor::IsInitResourceScaleCurveSuccess()
{
	if(Curve == nullptr)
	{
		const FName CurveName = TEXT("CurveFloat'/Game/Resources/Curve/Resource_Curve.Resource_Curve'");
		Curve = LoadObject<UCurveFloat>(nullptr, *CurveName.ToString());
	}

	return Curve != nullptr;
}

void ACropBaseActor::StartTimeline()
{
	Timeline.PlayFromStart();
}

void ACropBaseActor::StopTimeline()
{
	Timeline.Reverse();
}

void ACropBaseActor::TimelineUpdate(float scaleValue) const
{
	StaticMeshComponent->SetRelativeScale3D(FVector(scaleValue, scaleValue, scaleValue));
}

void ACropBaseActor::TimelineFinished()
{
	StaticMeshComponent->SetRelativeScale3D(FVector(1, 1, 1));
}

void ACropBaseActor::RegisterWithEntityManager()
{
	if(EntityManager != nullptr)
	{
		EntityManager->AddCrop(this);
	}
}

void ACropBaseActor::UnregisterWithEntityManager()
{
	if(EntityManager != nullptr)
	{
		EntityManager->RemoveCrop(this);
	}
}

float ACropBaseActor::Interact()
{
	Super::Interact();

	if(IsGrowthProcess)
	{
		return 0.f;
	}

	if(GrowthProgress >= 1.f)
	{
		return 0.f;
	}

	// growth
	IsGrowthProcess = true;
	CurrentGrowthStartTime = CurrentGrowthTime;

	if(CurrentGrowthTime + 4.f >= GrowthTime)
	{
		CurrentGrowthEndTime = CurrentGrowthStartTime + (GrowthTime - CurrentGrowthTime);
		IsHarvestablePostGrowth = true;
	}
	else
	{
		CurrentGrowthEndTime = CurrentGrowthStartTime + 4.f; // Action 한번에 4초 성장함
		IsHarvestablePostGrowth = false;
	}

	return 1.f;
}

TTuple<EResourceType, int> ACropBaseActor::CollectResource()
{
	GrowthProgress = 0.f;
	IsGrowthProcess = false;
	IsHarvestablePostGrowth = false;
	CurrentGrowthTime = 0.f;
	ChangeMesh(0);
	EntityManager->ChangeCropState(this, false, true);
	return MakeTuple(EResourceType::Food, HarvestAmount);
}

bool ACropBaseActor::IsReadyToHarvest() const
{
	return GrowthProgress >= 1.f;
}

bool ACropBaseActor::IsGrowth() const
{
	return IsGrowthProcess;
}

bool ACropBaseActor::CheckHarvestablePostGrowth() const
{
	return IsHarvestablePostGrowth;
}

void ACropBaseActor::AssignFarmer(AVillager* Farmer)
{
	AssignedFarmer = Farmer;
}

void ACropBaseActor::UnassignFarmer()
{
	AssignedFarmer = nullptr;
}

bool ACropBaseActor::HasFarmer()
{
	return AssignedFarmer != nullptr;
}

void ACropBaseActor::ChangeMesh(int index, bool isFirst)
{
	if(index >= 0 && index < MeshList.Num())
	{
		if(StaticMeshComponent->GetStaticMesh() != MeshList[index])
		{
			StaticMeshComponent->SetStaticMesh(MeshList[index]);
			if(isFirst == false)
			{
				StaticMeshComponent->SetVisibility(true);
				StartTimeline();
			}
			else
			{
				StaticMeshComponent->SetVisibility(false);
				RecalcBoxExtentDelegateHandle = AddReCalcBoxExtentDelegateHandle(this,
					&ACropBaseActor::RecalcBoxExtentCompleted);
			}
		}
	}
}

void ACropBaseActor::RecalcBoxExtentCompleted()
{
	RemoveReCalcBoxExtentDelegateHandle(RecalcBoxExtentDelegateHandle);
	StaticMeshComponent->SetVisibility(true);
	StartTimeline();
}
