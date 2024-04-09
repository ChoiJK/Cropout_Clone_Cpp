// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableBaseActor.generated.h"

class UBoxComponent;
class UTextureRenderTarget2D;
class UStaticMesh;
class UCurveFloat;
struct FTimeline;
struct FInteractableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API AInteractableBaseActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableBaseActor();

private:
	void InitializeWithInteractableInfo();
	void CheckOverlappingActor();
	void DoGrundBlend();

protected:
	FName Name;

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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	UMaterial* ShapeDrawMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Wooble_Curve; // timeline

	FTimeline Wooble_Timeline;

	virtual void BeginPlay() override;

	virtual void InitializeStaticMesh()
	{
	}

	void ReCalcBoxExtent() const;

	UMaterial* GetShapeDrawMaterial();

	void TransformToTexture(FVector2d InVec, FVector2d& OutVec, FVector2d& OutVec2) const;


	bool IsInitWoobleCurveSuccess();

	UFUNCTION()
	void Wooble_TimelineUpdate(float scaleValue) const;
	UFUNCTION()
	void Wooble_TimelineFinished();

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// returnVal : work time
	virtual float Interact()
	{
		return 0.0f;
	};

	virtual void SetInteractableInfo(const FInteractableInfo* InInfo);

	FName GetName() const
	{
		return Name;
	}

	UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}

	void PlayWobble(FVector InVec);
	void EndWooble();

	void SetEnableGroundBlend(bool InEnableGroundBlend);
};
