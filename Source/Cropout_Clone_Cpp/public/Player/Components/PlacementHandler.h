// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlacementHandler.generated.h"


class UGlobalSharedVariable;
class AInteractableBaseActor;
class ACropoutPlayer;

struct FInteractableInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROPOUT_CLONE_CPP_API UPlacementHandler : public UActorComponent
{
	GENERATED_BODY()

	ACropoutPlayer* Owner = nullptr;
	AInteractableBaseActor* PlacementTargetEntity = nullptr;

	UGlobalSharedVariable* GlobalSharedVariable = nullptr;
	UStaticMeshComponent* PlacementSMC = nullptr;

public:
	UPlacementHandler();

protected:
	virtual void BeginPlay() override;

public:
	void Initialize();
	bool CheckEntityAgainstPlacementTarget(const FInteractableInfo* info);
	void SetPlacementTargetEntity(AInteractableBaseActor* entity);
	void ReleasePlacementTargetEntity();
	void InitPlacementIMC();
	void ReleasePlacementIMC();
};
