// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBaseActor.generated.h"

class UBoxComponent;
class UTextureRenderTarget2D;
class UStaticMesh;

UCLASS()
class CROPOUT_CLONE_CPP_API AInteractableBaseActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableBaseActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	void ReCalcBoxExtent() const;


	UMaterial* ShapeDrawMaterial = nullptr;
	UMaterial* GetShapeDrawMaterial();

	void TransformToTexture(FVector2d InVec, FVector2d& OutVec, FVector2d& OutVec2) const;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Interact()
	{
	};

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	TObjectPtr<UTextureRenderTarget2D> RT_Draw;

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	bool EnableGroundBlend = true;

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	float OutlineDraw = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Progression")
	bool RequireBuild = false;

	UPROPERTY(EditAnywhere, Category = "Progression")
	float ProgressionState = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Progression")
	TArray<UStaticMesh*> MeshList;

	UPROPERTY(EditAnywhere, Category = "Spawn Info")
	float BoundGap = 0.0f;
};
