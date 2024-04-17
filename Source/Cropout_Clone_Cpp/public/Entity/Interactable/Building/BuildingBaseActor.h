// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Interactable/InteractableBaseActor.h"
#include "BuildingBaseActor.generated.h"

class AVillager;
class UBoxComponent;
class UNavModifierComponent;

UCLASS()
class CROPOUT_CLONE_CPP_API ABuildingBaseActor : public AInteractableBaseActor
{
	GENERATED_BODY()

	UBoxComponent* NavBlocker;
	void ChangeMesh(int index);

	TArray<AVillager*> Builders;
	float RemainingBuildTimeSec = 0.0f;
	float BuildProgress = 0.0f;
	FTimerHandle BuildTimerHandle;

public:
	ABuildingBaseActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;
	virtual void SetInteractableInfo(const FInteractableInfo* InInfo) override;

	virtual float Interact() override;
	void BuildTick();
	bool IsRequireBuild() const;
	void AddBuilder(AVillager* Builder);
	void RemoveBuilder(AVillager* Builder);

protected:
	virtual void BeginPlay() override;
	virtual void InitializeStaticMesh() override;
	virtual void RegisterWithEntityManager() override;
};
