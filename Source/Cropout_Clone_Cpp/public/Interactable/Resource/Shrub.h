// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Resource/ResourceBaseActor.h"
#include "Shrub.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API AShrub : public AResourceBaseActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShrub();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
