// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Data/VillagerJob.h"
#include "Villager.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
class USkeletalMesh;
class UCropoutGameInstance;
class UAnimBlueprint;
class ACropoutGameMode;

UCLASS(BlueprintType)
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFloatingPawnMovement* Movement;

	UFUNCTION(BlueprintCallable)
	UFloatingPawnMovement* GetMovement() const { return Movement; }

	AVillager();

protected:
	FTimerHandle EatTimerHandle;
	void Eat();
	virtual void BeginPlay() override;

public:
	virtual void Destroyed() override;

private:
	UCropoutGameInstance* GameInstance;
	ACropoutGameMode* GameMode;
	EVillagerJobType CurrentJobType = EVillagerJobType::None;
	FName CurrentJobName;

	void ChangeJob(EVillagerJobType jobType);
	void ResetJobState();
	void StopJob();

	void AsyncJobResourceLoaded(FName jobName);

	TMap<FName, FVillagerJob> MapVillagerJob;
	UAnimMontage* WorkAnim = nullptr;
	UStaticMesh* TargetTool = nullptr;
	UBehaviorTree* ActiveBehaviorTree = nullptr;
	AActor* TargetRef = nullptr;

	UAnimBlueprint* BodyAnimBlueprint = nullptr;
	UAnimBlueprint* HairAnimBlueprint = nullptr;
	UAnimBlueprint* HatAnimBlueprint = nullptr;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Action(AActor* jobAction);
	void SetDefaultJob();
	AActor* GetTargetRef() const { return TargetRef; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 0;
};
