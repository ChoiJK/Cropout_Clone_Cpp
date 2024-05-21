// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Data/InGameSaveData.h"
#include "Enum/InteractableType.h"
#include "GameFramework/Actor.h"
#include "Player/Components/PlacementHandler.h"
#include "InteractableBaseActor.generated.h"

class UBoxComponent;
class UTextureRenderTarget2D;
class UPlacementHandler;
class UStaticMesh;
class UCurveFloat;
class FDelegateHandle;
class UEntityManager;
struct FTimeline;
struct FInteractableInfo;

DECLARE_MULTICAST_DELEGATE(FReCalcBoxExtentDelegate);

UCLASS(Abstract)
class CROPOUT_CLONE_CPP_API AInteractableBaseActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableBaseActor();

private:
	TSharedPtr<FReCalcBoxExtentDelegate> ReCalcBoxExtentDelegate = nullptr;

	void InitializeWithInteractableInfo();
	void CheckOverlappingActor();
	void DoGrundBlend();

protected:
	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	EInteractableType InteractableType = EInteractableType::None;

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	TObjectPtr<UTextureRenderTarget2D> RT_Draw;

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	bool EnableGroundBlend = true;

	UPROPERTY(EditAnywhere, Category = "Ground Blend")
	float OutlineDraw = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Progression")
	bool RequireBuild = false;

	UPROPERTY(EditAnywhere, Category = "Progression")
	int BuildTimeSec = 10;

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

	virtual void InitializeStaticMesh(int index = -1)
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

	UPROPERTY()
	UEntityManager* EntityManager = nullptr;

	UFUNCTION()
	virtual void RegisterWithEntityManager() PURE_VIRTUAL(AInteractableBaseActor::RegisterWithEntityManager,);

	void InternalGetSaveData(FSpawnEntitySaveData& OutData);

public:
	virtual void Tick(float DeltaSeconds) override;

	template <typename T>
	FDelegateHandle AddReCalcBoxExtentDelegateHandle(T* ClassInstance,
		void (T::*InFunc)())
	{
		static_assert(std::is_base_of_v<UObject, T>, "T must be derived from UObject");
		if(!ReCalcBoxExtentDelegate)
		{
			ReCalcBoxExtentDelegate = MakeShareable(new FReCalcBoxExtentDelegate());
		}

		return ReCalcBoxExtentDelegate->AddUObject(ClassInstance, InFunc);
	}

	void RemoveReCalcBoxExtentDelegateHandle(FDelegateHandle delegateHandle)
	{
		if(!ReCalcBoxExtentDelegate)
		{
			return;
		}

		ReCalcBoxExtentDelegate->Remove(delegateHandle);
	}


	virtual void OnConstruction(const FTransform& Transform) override;

	// returnVal : work time
	virtual float Interact()
	{
		return 0.0f;
	}

	virtual void SetInteractableInfo(const FInteractableInfo* InInfo, const FSpawnEntitySaveData* LoadData = nullptr);

	UFUNCTION()
	virtual FSpawnEntitySaveData GetSaveData() PURE_VIRTUAL(AInteractableBaseActor::GetSaveData,
		{return FSpawnEntitySaveData();});

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

	/// Sort Temp
	float DistSquared = 0.0f;
	void CalcTemporaryDistanceFromLocation(FVector InLocation);

	float GetDistSquared() const
	{
		return DistSquared;
	}
};
