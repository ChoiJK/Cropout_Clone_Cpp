// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableBaseActor.h"
#include "BuildingBaseActor.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API ABuildingBaseActor : public AInteractableBaseActor
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
