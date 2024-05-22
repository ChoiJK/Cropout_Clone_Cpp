// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "Components/TimelineComponent.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "Enum/ResourceType.h"
#include "ResourceBaseActor.generated.h"


class UCurveFLoat;
class FGlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API AResourceBaseActor : public AInteractableBaseActor
{
	GENERATED_BODY()

public:
	AResourceBaseActor();
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	bool IsInitResourceScaleCurveSuccess();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	FTimeline Timeline;
	void StartTimeline();
	void StopTimeline();

	UFUNCTION()
	void TimelineUpdate(float scaleValue) const;
	UFUNCTION()
	void TimelineFinished();
	virtual void RegisterWithEntityManager() override;
	virtual void UnregisterWithEntityManager() override;

public:
	void DoScaleUp(float delay);
	void DoDeath();
	virtual void SetInteractableInfo(const FInteractableInfo* InInfo,
		const FSpawnEntitySaveData* LoadData = nullptr) override;
	virtual FSpawnEntitySaveData GetSaveData() override;
	virtual void BeginDestroy() override;
	virtual void InitializeStaticMesh(int index = -1) override;
	virtual float Interact() override;
	TTuple<EResourceType, int> CollectResource();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Curve; // timeline

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	EResourceType ResourceType = EResourceType::None;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	float ResourceAmount = 100.0f;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	float CollectionTime = 3.0f;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	int CollectionValue = 10;

	UPROPERTY(EditAnywhere, Category="Progression")
	bool UseRandomMesh = false;
};
