// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractableBaseActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Canvas.h"
#include "cmath"

AInteractableBaseActor::AInteractableBaseActor()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
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

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> TextureRenderTargetAsset(
		TEXT("TextureRenderTarget2D'/Game/Blueprint/Extras/RT_GrassMove.RT_GrassMove'"));
	if(TextureRenderTargetAsset.Succeeded())
	{
		RT_Draw = TextureRenderTargetAsset.Object;
	}
}

// Called when the game starts or when spawned
void AInteractableBaseActor::BeginPlay()
{
	Super::BeginPlay();

	ReCalcBoxExtent();

	if(UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
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

			// Check if actor is overlapped with other actors
			// @ CHECK : Remove when using PCG
			TArray<AActor*> OverlappingActors;
			GetOverlappingActors(OverlappingActors, StaticClass());
			const FName targetTag = TEXT("PlacementMode");
			bool isDestroy = false;
			for(const AActor* actor : OverlappingActors)
			{
				if(actor->ActorHasTag(targetTag))
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
		}));
	}
}

void AInteractableBaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ReCalcBoxExtent();
}

// @todo : Event Play Wobble
// @todo : Event End Wobble

// @todo : Placement Mode

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
