// Fill out your copyright notice in the Description page of Project Settings.

#include "Entity/Interactable/InteractableBaseActor.h"
#include "Entity/EntityManager.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Canvas.h"
#include "cmath"
#include "Core/CropoutGameInstance.h"
#include "Data/InteractableInfo.h"

AInteractableBaseActor::AInteractableBaseActor()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->SetCanEverAffectNavigation(true);


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	FCollisionResponseContainer collisionResponse;
	collisionResponse.SetResponse(ECC_Visibility, ECR_Overlap);
	collisionResponse.SetResponse(ECC_Camera, ECR_Overlap);
	collisionResponse.SetResponse(ECC_WorldDynamic, ECR_Overlap);
	collisionResponse.SetResponse(ECC_Pawn, ECR_Overlap);

	collisionResponse.SetResponse(ECC_WorldStatic, ECR_Ignore);
	collisionResponse.SetResponse(ECC_PhysicsBody, ECR_Ignore);
	collisionResponse.SetResponse(ECC_Vehicle, ECR_Ignore);
	collisionResponse.SetResponse(ECC_Destructible, ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannels(collisionResponse);

	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if(IsInitWoobleCurveSuccess() == false)
	{
		Destroy();
		return;
	}
}

void AInteractableBaseActor::SetInteractableInfo(const FInteractableInfo* InInfo, const FSpawnEntitySaveData* LoadData)
{
	Name = InInfo->Name;
	EntityID = LoadData ? LoadData->EntityID : "";
	InteractableType = InInfo->InteractableType;
	EnableGroundBlend = InInfo->EnableGroundBlend;
	OutlineDraw = InInfo->OutlineDraw;
	RequireBuild = InInfo->RequirBuildable;
	BuildTimeSec = InInfo->BuildTimeSec;
	ProgressionState = 0.f;

	BoundGap = InInfo->BoundGap;

	UCropoutGameInstance* cropoutGameInstance = GetGameInstance<UCropoutGameInstance>();
	EntityManager = cropoutGameInstance->GetLevelManager<UEntityManager>();
	TArray<FSoftObjectPath> MeshPathList;
	for(const TSoftObjectPtr<UStaticMesh>& mesh : InInfo->MeshList)
	{
		MeshPathList.Add(mesh.ToSoftObjectPath());
	}

	int MeshIndex = LoadData ? LoadData->MeshIndex : -1;

	RegisterWithEntityManager();

	cropoutGameInstance->GetStreamableManager().RequestAsyncLoad(MeshPathList,
		FStreamableDelegate::CreateLambda([MeshPathList, MeshIndex, this]()
		{
			for(FSoftObjectPath meshPath : MeshPathList)
			{
				this->MeshList.Add(Cast<UStaticMesh>(meshPath.TryLoad()));
			}

			InitializeStaticMesh(MeshIndex);

			if(const UWorld* world = GetWorld())
			{
				world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
				{
					InitializeWithInteractableInfo();
				}));
			}
		}));
}

void AInteractableBaseActor::InternalGetSaveData(FSpawnEntitySaveData& OutData)
{
	OutData.EntityID = EntityID;
	OutData.InteractableName = Name;
	OutData.InteractableType = InteractableType;
	OutData.MeshIndex = MeshList.Find(StaticMeshComponent->GetStaticMesh());
	OutData.Location = GetActorLocation();
	OutData.Rotation = GetActorRotation();
	OutData.Scale = GetActorScale();
}

void AInteractableBaseActor::InitializeWithInteractableInfo()
{
	ReCalcBoxExtent();

	if(UWorld* world = GetWorld())
	{
		// Placement Mode일경우 EnableGroundBlend를 false로 강제설정하기때문에 한프레임 뒤에 실행되어야함
		world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			DoGrundBlend();
			CheckOverlappingActor();

			// Placement Mode일경우 EntityManager에 등록하지 않음
			const FName targetTag = TEXT("PlacementMode");
			if(ActorHasTag(targetTag))
			{
				UnregisterWithEntityManager();
			}
		}));
	}
}

void AInteractableBaseActor::DoGrundBlend()
{
	if(EnableGroundBlend && OutlineDraw > 0.f)
	{
		// Remove grass and paint dirty around asset
		UCanvas* Canvas = nullptr;
		FVector2D Size;
		FDrawToRenderTargetContext Context;
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RT_Draw, Canvas, Size, Context);
		UMaterialInterface* Material = StaticMeshComponent->GetMaterial(0);

		FVector2D ScreenPosition;
		FVector2D ScreenSize;
		TransformToTexture(Size, ScreenPosition, ScreenSize);

		UMaterial* shapeDrawMaterial = GetShapeDrawMaterial();
		shapeDrawMaterial->EnsureIsComplete();

		FVector2D coordinatePosition = FVector2D(0, 0);
		FVector2D coordinateSize = FVector2D(1, 1);
		FCanvasTileItem TileItem(ScreenPosition, shapeDrawMaterial->GetRenderProxy(), ScreenSize,
			coordinatePosition, coordinatePosition + coordinateSize);
		TileItem.Rotation = FRotator(0, 0, 0);
		TileItem.PivotPoint = FVector2D(0.5f, 0.5f);
		TileItem.SetColor(FLinearColor::White);
		Canvas->DrawItem(TileItem);
		Canvas->K2_DrawMaterial(shapeDrawMaterial, ScreenPosition, ScreenSize, FVector2D(0, 0));

		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
	}
}

void AInteractableBaseActor::CheckOverlappingActor()
{
	// Check if actor is overlapped with other actors
	// @ CHECK : Remove when using PCG
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, StaticClass());
	const FName targetTag = TEXT("PlacementMode");
	bool isDestroy = false;
	for(const AActor* actor : OverlappingActors)
	{
		if(actor->ActorHasTag(targetTag) == false)
		{
			const bool isOverlapped = FVector::Distance(GetActorLocation(), actor->GetActorLocation()) < 5;
			if(isOverlapped)
			{
				isDestroy = true;
				break;
			}
		}
	}

	if(isDestroy)
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void AInteractableBaseActor::BeginPlay()
{
	Super::BeginPlay();

	RT_Draw = UCropoutGameInstance::GetInstance()->GetGlobalSharedVariable()->GetRT_Draw();

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("Wooble_TimelineUpdate"));
	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("Wooble_TimelineFinished"));

	Wooble_Timeline.AddInterpFloat(Wooble_Curve, TimelineProgress);
	Wooble_Timeline.SetTimelineFinishedFunc(TimelineFinished);
}

bool AInteractableBaseActor::IsInitWoobleCurveSuccess()
{
	if(Wooble_Curve == nullptr)
	{
		const FName CurveName = TEXT("CurveFloat'/Game/Resources/Curve/Woodle_Curve.Woodle_Curve'");
		Wooble_Curve = LoadObject<UCurveFloat>(nullptr, *CurveName.ToString());
	}

	return Wooble_Curve != nullptr;
}

void AInteractableBaseActor::Wooble_TimelineUpdate(float scaleValue) const
{
	StaticMeshComponent->SetScalarParameterValueOnMaterials("Wobble", scaleValue);
}

void AInteractableBaseActor::Wooble_TimelineFinished()
{
	StaticMeshComponent->SetScalarParameterValueOnMaterials("Wobble", 0.f);
}

void AInteractableBaseActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Wooble_Timeline.IsPlaying())
	{
		Wooble_Timeline.TickTimeline(DeltaSeconds);
	}
}

void AInteractableBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ReCalcBoxExtent();
}

void AInteractableBaseActor::PlayWobble(FVector InVec)
{
	FVector temVec = InVec - GetActorLocation();
	temVec.Normalize();
	StaticMeshComponent->SetVectorParameterValueOnMaterials("Wobble Vector", temVec);
	Wooble_Timeline.PlayFromStart();
}

void AInteractableBaseActor::EndWooble()
{
	StaticMeshComponent->SetVectorParameterValueOnMaterials("Wobble Vector", FVector(0, 0, 0));
	Wooble_Timeline.Stop();
}

void AInteractableBaseActor::SetEnableGroundBlend(bool InEnableGroundBlend)
{
	EnableGroundBlend = InEnableGroundBlend;
}

void AInteractableBaseActor::ReCalcBoxExtent() const
{
	if(StaticMeshComponent == nullptr || BoxComponent == nullptr)
	{
		return;
	}

	FVector boundsMin, boundsMax;
	StaticMeshComponent->GetLocalBounds(boundsMin, boundsMax);
	boundsMax /= 100;
	boundsMax.X = std::round(boundsMax.X);
	boundsMax.Y = std::round(boundsMax.Y);
	boundsMax.Z = std::round(boundsMax.Z);
	boundsMax *= 100; //Step
	double maxXY = std::max(boundsMax.X, boundsMax.Y);
	maxXY = std::max(maxXY, 100.0);
	boundsMax.X = maxXY;
	boundsMax.Y = maxXY;
	boundsMax.Z = std::max(boundsMax.Z, 100.0);

	float boundGap = BoundGap * 100;
	boundsMax += FVector(boundGap, boundGap, boundGap);

	BoxComponent->SetBoxExtent(boundsMax);
	BoxComponent->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1, 0, 0)));


	if(ReCalcBoxExtentDelegate != nullptr)
	{
		ReCalcBoxExtentDelegate->Broadcast();
	}
}

void AInteractableBaseActor::TransformToTexture(FVector2d InVec, FVector2d& OutVec, FVector2d& OutVec2) const
{
	FVector actorLocation = GetActorLocation();
	actorLocation = actorLocation + FVector(10000.f, 10000.f, 10000.f);
	actorLocation /= 20000.f;
	actorLocation *= InVec.X;

	FVector origin = FVector(0, 0, 0);
	FVector boxExtent = FVector(0, 0, 0);
	GetActorBounds(false, origin, boxExtent);
	//boxExtent *= 0.25f;
	float minValue = std::min(boxExtent.X, boxExtent.Y);
	minValue /= 10000.f;
	minValue = minValue * InVec.X * OutlineDraw;
	OutVec2 = FVector2d(minValue, minValue);
	minValue /= 2.f;

	actorLocation = actorLocation - FVector(minValue, minValue, minValue);
	OutVec = FVector2d(actorLocation.X, actorLocation.Y);
}

UMaterial* AInteractableBaseActor::GetShapeDrawMaterial()
{
	if(ShapeDrawMaterial == nullptr)
	{
		FName path = TEXT("Material'/Game/Environment/Materials/M_ShapeDraw.M_ShapeDraw'");
		ShapeDrawMaterial = LoadObject<UMaterial>(nullptr, *path.ToString());
	}

	return ShapeDrawMaterial;
}

void AInteractableBaseActor::CalcTemporaryDistanceFromLocation(FVector InLocation)
{
	DistSquared = FVector::DistSquared(GetActorLocation(), InLocation);
}
