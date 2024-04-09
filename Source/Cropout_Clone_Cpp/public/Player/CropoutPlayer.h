// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CropoutPlayer.generated.h"

class UCropoutGameInstance;
class ASpawner;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USphereComponent;
class UFloatingPawnMovement;
class FGlobalEventDispatcher;
class ACropoutPlayerController;
class AInteractableBaseActor;

class UMovementInputHandler;
class UVillagerHandler;
class UPlacementHandler;

struct FTimerHandle;
struct FBuildableInfo;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayer : public APawn
{
	GENERATED_BODY()

public:
	ACropoutPlayer();

private:
	UCropoutGameInstance* GameInstance = nullptr;
	TWeakPtr<FGlobalEventDispatcher> GlobalEventDispatcher = nullptr;
	ACropoutPlayerController* PlayerController = nullptr;
	ASpawner* Spawner = nullptr;
	AActor* HoverActor = nullptr;

	ASpawner* GetSpawner();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	FTimerHandle ClosestHoverCheckTimerHandle;
	void ClosestHoverCheck();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CursorMesh = nullptr;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collision = nullptr;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement = nullptr;

	UMovementInputHandler* MovementInputHandler = nullptr;
	UVillagerHandler* VillagerHandler = nullptr;
	UPlacementHandler* PlacementHandler = nullptr;

	ACropoutPlayerController* GetPlayerController();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnChangedInputType();

	void SetHoverActor(AActor* actor) { HoverActor = actor; }
	AActor* GetHoverActor() { return HoverActor; }

	void BeginBuild(const FBuildableInfo* buildableInfo);
	void EndBuild();
};
