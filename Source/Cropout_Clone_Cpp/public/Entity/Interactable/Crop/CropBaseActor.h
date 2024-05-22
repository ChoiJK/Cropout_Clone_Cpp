// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "Components/TimelineComponent.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "Enum/ResourceType.h"
#include "CropBaseActor.generated.h"

class AVillager;
class UCurveFLoat;
class FGlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropBaseActor : public AInteractableBaseActor
{
	GENERATED_BODY()

public:
	ACropBaseActor();
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	bool IsGrowthProcess = false;;
	float GrowthProgress; // Growth 진행도
	float CurrentGrowthStartTime;
	float CurrentGrowthEndTime;
	float CurrentGrowthTime; // 현재까지 Growth 중첩된 시간
	bool IsHarvestablePostGrowth = false;
	bool IsInitResourceScaleCurveSuccess();

	AVillager* AssignedFarmer = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitializeStaticMesh(int index = -1) override;
	FTimeline Timeline;
	void StartTimeline();
	void StopTimeline();

	UFUNCTION()
	void TimelineUpdate(float scaleValue) const;
	UFUNCTION()
	void TimelineFinished();
	virtual void RegisterWithEntityManager() override;
	virtual void UnregisterWithEntityManager() override;

	void ChangeMesh(int index, bool isFirst = false);
	FDelegateHandle RecalcBoxExtentDelegateHandle;
	UFUNCTION()
	void RecalcBoxExtentCompleted();

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetInteractableInfo(const FInteractableInfo* InInfo,
		const FSpawnEntitySaveData* LoadData = nullptr) override;
	virtual FSpawnEntitySaveData GetSaveData() override;
	virtual void BeginDestroy() override;
	virtual float Interact() override;
	TTuple<EResourceType, int> CollectResource();

	bool IsReadyToHarvest() const;
	bool IsGrowth() const;
	bool CheckHarvestablePostGrowth() const;

	void AssignFarmer(AVillager* Farmer);
	void UnassignFarmer();
	bool HasFarmer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Curve; // timeline

	UPROPERTY(EditAnywhere, Category="Crop Properties")
	float GrowthTime = 10.0f;

	UPROPERTY(EditAnywhere, Category="Crop Properties")
	int HarvestAmount = 30;
};
