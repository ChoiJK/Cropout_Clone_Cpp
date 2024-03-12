#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VillagerHandler.generated.h"

class UNavigationPath;
class UInputAction;
class UInputComponent;
class ACropoutPlayer;
class UEnhancedInputLocalPlayerSubsystem;
class UNiagaraSystem;
class UNiagaraComponent;
class UInputMappingContext;

struct FInputActionValue;
struct FTimerHandle;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROPOUT_CLONE_CPP_API UVillagerHandler : public UActorComponent
{
	GENERATED_BODY()
	ACropoutPlayer* Owner = nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;

	// Villager
	AActor* VillagerActor = nullptr;
	AActor* SelectedVillager = nullptr;
	AActor* VillagerAction = nullptr;

	void VillagerSelect(AActor* selectedVillager);
	void VillagerRelease();

	AActor* VillagerOverlapCheck();

	// Input
	UInputMappingContext* IMC_Villager = nullptr;
	UInputAction* IA_VillagerAction = nullptr;

	void OnVillagerActionTriggered(const FInputActionValue& Value);
	void OnVillagerAction(const FInputActionValue& Value);
	void OnVillagerActionEnd(const FInputActionValue& Value);

	// Path Effect
	UNiagaraSystem* NS_PathEffect;
	UNiagaraComponent* NS_PathEffectComponent;
	FTimerHandle PathTimerHandle;
	void UpdatePath();

public:
	UVillagerHandler();

protected:
	virtual void BeginPlay() override;

public:
	void Initialize();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
};
