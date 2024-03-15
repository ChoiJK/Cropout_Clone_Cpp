// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/Resource/ResourceBaseActor.h"
#include "Components/TimelineComponent.h"
#include "Core/CropoutGameInstance.h"

AResourceBaseActor::AResourceBaseActor()
{
	if(IsInitResourceScaleCurveSuccess() == false)
	{
		Destroy();
		return;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AResourceBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FName resurceType = FName(EnumToString(ResourceType));
	Tags.Add(resurceType);

	if(UseRandomMesh)
	{
		StaticMeshComponent->SetStaticMesh(MeshList[FMath::RandRange(0, MeshList.Num() - 1)]);
	}
}

// Called when the game starts or when spawned
void AResourceBaseActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));
	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("TimelineFinished"));

	Timeline.AddInterpFloat(Curve, TimelineProgress);
	Timeline.SetTimelineFinishedFunc(TimelineFinished);
}

void AResourceBaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AResourceBaseActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timeline.TickTimeline(DeltaSeconds);
}


bool AResourceBaseActor::IsInitResourceScaleCurveSuccess()
{
	if(Curve == nullptr)
	{
		const FName CurveName = TEXT("CurveFloat'/Game/Resources/Curve/Resource_Curve.Resource_Curve'");
		Curve = LoadObject<UCurveFloat>(nullptr, *CurveName.ToString());
	}

	return Curve != nullptr;
}

void AResourceBaseActor::StartTimeline()
{
	Timeline.PlayFromStart();
}

void AResourceBaseActor::StopTimeline()
{
	Timeline.Reverse();
}

void AResourceBaseActor::TimelineUpdate(float scaleValue) const
{
	StaticMeshComponent->SetRelativeScale3D(FVector(scaleValue, scaleValue, scaleValue));
}

void AResourceBaseActor::TimelineFinished()
{
	StaticMeshComponent->SetRelativeScale3D(FVector(1, 1, 1));
}

void AResourceBaseActor::DoScaleUp(float delay)
{
	StaticMeshComponent->SetHiddenInGame(true);

	if(UWorld* world = GetWorld())
	{
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			StaticMeshComponent->SetRelativeScale3D(FVector::Zero());
			StaticMeshComponent->SetHiddenInGame(false);

			StartTimeline();
		}), delay, false);
	}
}

void AResourceBaseActor::DoDeath()
{
	Destroy();
}

float AResourceBaseActor::Interact()
{
	Super::Interact();

	return CollectionTime;
}

TTuple<EResourceType, int> AResourceBaseActor::CollectResource()
{
	EndWooble();

	int resultValue = 0;
	if(ResourceAmount <= CollectionValue)
	{
		resultValue = ResourceAmount;
		ResourceAmount = 0;
	}
	else
	{
		resultValue = CollectionValue;
		ResourceAmount -= CollectionValue;
	}

	if(ResourceAmount <= 0)
	{
		DoDeath();
	}

	return MakeTuple(ResourceType, resultValue);
}
