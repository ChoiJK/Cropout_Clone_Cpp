// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Data/VillagerJob.h"
#include "Villager.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
class USkeletalMesh;

UCLASS()
class CROPOUT_CLONE_CPP_API AVillager : public APawn
{
	GENERATED_BODY()

	static TArray<USkeletalMesh*> HairMeshes;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ToolMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* ShadowDecal;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* Movement;

public:
	AVillager();

protected:
	FTimerHandle EatTimerHandle;
	void Eat();
	virtual void BeginPlay() override;

	EVillagerJobType JobType = EVillagerJobType::None;
	void ChangeJob(EVillagerJobType jobType);

	TMap<FName, FVillagerJob> MapVillagerJob;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Action(AActor* jobAction);
};
