// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Data/VillagerJob.h"
#include "Enum/ResourceType.h"
#include "Villager.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
class USkeletalMesh;
class UCropoutGameInstance;
class UAnimBlueprint;
class AInGameGameMode;
struct FVillagerSaveData;

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

	UPROPERTY()
	UStaticMesh* CrateMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFloatingPawnMovement* Movement;

	UFUNCTION(BlueprintCallable)
	UFloatingPawnMovement* GetMovement() const { return Movement; }

	AVillager();

protected:
	FTimerHandle WorkAnimTimerHandle;
	FTimerHandle EatTimerHandle;
	void Eat();
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	UCropoutGameInstance* GameInstance;
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

	UFUNCTION()
	void PlayMontageCallback(FName NotifyName);

	EResourceType StoreResourceType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StoreAmount;

public:
	FVillagerSaveData GetSaveData();
	void LoadSaveData(FVillagerSaveData* saveData);
	bool CheckHasResource() const;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayWorkAnim(float delay);
	void EndWorkAnim();
	void Action(AActor* jobAction);
	void SetDefaultJob();
	AActor* GetTargetRef() const { return TargetRef; }

	void StoreResource(EResourceType type, int amount);
	void DeliverResource();
};
