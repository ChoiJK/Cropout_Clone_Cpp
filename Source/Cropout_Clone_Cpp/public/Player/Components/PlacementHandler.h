// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlacementHandler.generated.h"


class UGlobalSharedVariable;
class AInteractableBaseActor;
class ACropoutPlayer;
class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class UInputAction;
class UInputComponent;

struct FInteractableInfo;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROPOUT_CLONE_CPP_API UPlacementHandler : public UActorComponent
{
	GENERATED_BODY()

	ACropoutPlayer* Owner = nullptr;
	AInteractableBaseActor* PlacementTargetEntity = nullptr;

	UGlobalSharedVariable* GlobalSharedVariable = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlacementSMC = nullptr;

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
	UInputMappingContext* IMC_BuildMode = nullptr;
	UInputAction* IA_BuildMove = nullptr;

public:
	UPlacementHandler();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBuildMovePressed(const FInputActionValue& Value);

public:
	void Initialize();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	bool CheckEntityAgainstPlacementTarget(const FInteractableInfo* info);
	void SetPlacementTargetEntity(AInteractableBaseActor* entity);
	void ReleasePlacementTargetEntity();
	void InitPlacementIMC();
	void ReleasePlacementIMC();
};
