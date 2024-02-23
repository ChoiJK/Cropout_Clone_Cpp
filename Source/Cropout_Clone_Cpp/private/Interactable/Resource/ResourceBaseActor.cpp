// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/Resource/ResourceBaseActor.h"
#include "Components/TimelineComponent.h"
#include "Core/CropoutGameInstance.h"

AResourceBaseActor::AResourceBaseActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AResourceBaseActor Constructor"));
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

	auto gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	if(gameInstance)
	{
		EventDispatcher = gameInstance->GetGlobalEventDispatcher();
	}

	if(const TSharedPtr<GlobalEventDispatcher> dispatcher = EventDispatcher.Pin())
	{
		ScaleUpHandle = dispatcher->AddListenerUObject(EGlobalEventType::ScaleUp, this, &AResourceBaseActor::ScaleUp);
		if(ScaleUpHandle.IsValid() == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to add listener for IslandGenComplete"));
		}
	}

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));
	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("TimelineFinished"));

	Timeline.AddInterpFloat(Curve, TimelineProgress);
	Timeline.SetTimelineFinishedFunc(TimelineFinished);
}

void AResourceBaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(const TSharedPtr<GlobalEventDispatcher> dispatcher = EventDispatcher.Pin())
	{
		dispatcher->RemoveListener(EGlobalEventType::ScaleUp, ScaleUpHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void AResourceBaseActor::ScaleUp()
{
	StaticMeshComponent->SetHiddenInGame(true);

	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			StaticMeshComponent->SetRelativeScale3D(FVector::Zero());
			StaticMeshComponent->SetHiddenInGame(false);

			StartTimeline();
		}));
	}
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
	Timeline.Play();
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
}

void AResourceBaseActor::DoDeath()
{
	Destroy();
}

void AResourceBaseActor::Interact()
{
	Super::Interact();
}

// @todo : Event Scale Up
