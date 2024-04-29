#pragma once

#include "CoreMinimal.h"
#include "StateMachineBase.generated.h"

UENUM()
enum class ECropoutState : uint8
{
	None UMETA(DisplayName = "None"),
	Initialize UMETA(DisplayName = "Initialize"),
	Splash UMETA(DisplayName = "Splash"),
	MainMenu UMETA(DisplayName = "MainMenu"),
	InGame UMETA(DisplayName = "InGame"),
	InGamePause UMETA(DisplayName = "InGamePause"),
	InGameResult UMETA(DisplayName = "InGameResult"),
	Exit UMETA(DisplayName = "Exit"),
};

UENUM()
enum class EStateTrigger : uint8
{
	DoInitialize UMETA(DisplayName = "DoInitialize"),
	InitializeSuccess UMETA(DisplayName = "InitializeSuccess"),
	SplashComplete UMETA(DisplayName = "SplashComplete"),
	NewGame UMETA(DisplayName = "NewGame"),
	LoadGame UMETA(DisplayName = "LoadGame"),
	PauseGame UMETA(DisplayName = "PauseGame"),
	ResumeGame UMETA(DisplayName = "ResumeGame"),
	RestartGame UMETA(DisplayName = "RestartGame"),
	WinGame UMETA(DisplayName = "WinGame"),
	LoseGame UMETA(DisplayName = "LoseGame"),
	ToMainMenu UMETA(DisplayName = "ToMainMenu"),
	ExitGame UMETA(DisplayName = "ExitGame"),
};

UENUM()
enum class EStateProcessType : uint8
{
	None UMETA(DisplayName = "None"),
	Begin UMETA(DisplayName = "Begin"),
	Update UMETA(DisplayName = "Update"),
	End UMETA(DisplayName = "End"),
};

class UStateTransitionProcessor;

USTRUCT()
struct FStateTransition
{
	GENERATED_BODY()

	FStateTransition();
	FStateTransition(EStateTrigger trigger, ECropoutState toState, UStateTransitionProcessor* transitionProcessor);

	UPROPERTY()
	EStateTrigger Trigger;

	UPROPERTY()
	ECropoutState ToState;

	UPROPERTY()
	UStateTransitionProcessor* TransitionProcessor;
};

USTRUCT()
struct FStateTransitions
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FStateTransition> Transitions;
};

using StateProcessFunction = TFunction<void()>;
