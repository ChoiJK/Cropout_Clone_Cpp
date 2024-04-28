// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ApplicationState/StateMachineBase.h"
#include "Manager/Base/SystemManager.h"
#include "ApplicationStateManager.generated.h"

//  ┌────┐      ┌──────────┐     
//  │None├─────►│Initialize│     
//  └────┘      └────┬─────┘     
//  				 ▼           
//   ┌────────────┐ ┌──────┐       
//   │InGameResult│ │Splash│       
//   └───┬──┬────┬┘ └─┬────┘       
//    ▲  │  │    └────┼──────┐    
//    │  │  └───┐     │      │    
//    │  ▼      ▼     ▼      ▼    
//   ┌┴─────┐  ┌────────┐  ┌────┐ 
//   │InGame│◄─┤MainMenu├─►│Exit│ 
//   └┬─────┘  └────────┘  └────┘ 
//    │  ▲         ▲         ▲    
//    │  └─────┐   │         │    
//    │   ┌────┴───┴──┐      │    
//    └──►│InGamePause├──────┘    
//        └───────────┘            

// None -> Initialize		: DoInitialize
// Initialize -> Splash		: InitializeSuccess
// Splash -> MainMenu		: SplashComplete
// MainMenu -> InGame		: NewGame
// MainMenu -> InGame		: LoadGame
// MainMenu -> Exit			: ExitGame
// InGame -> InGamePause	: PauseGame
// InGame -> InGameResult	: EndOfGame
// InGamePause -> InGame	: ResumeGame
// InGamePause -> InGame	: RestartGame
// InGamePause -> MainMenu	: ToMainMenu
// InGamePause -> Exit		: ExitGame
// InGameResult -> InGame	: RestartGame
// InGameResult -> MainMenu	: ToMainMenu
// InGameResult -> Exit		: ExitGame

class UStateUpdater;
struct FStateTransitions;

UCLASS()
class CROPOUT_CLONE_CPP_API UApplicationStateManager : public USystemManager
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ECropoutState, UStateUpdater*> StateUpdaters;

	UPROPERTY()
	TMap<ECropoutState, FStateTransitions> TransitionProcessors;

	ECropoutState CurrentState;

	void InitializeStateUpdaters();
	void AddTransitionProcessors(ECropoutState state, FStateTransition transitions);

	void Initialize_None_To_Initialize_With_DoInitialize();
	void Initialize_Initialize_To_Splash_With_InitializeSuccess();
	void Initialize_Splash_To_MainMenu_With_SplashComplete();
	void Initialize_MainMenu_To_InGame_With_NewGame();
	void Initialize_MainMenu_To_InGame_With_LoadGame();
	void Initialize_MainMenu_To_Exit_With_ExitGame();
	void Initialize_InGame_To_InGamePause_With_PauseGame();
	void Initialize_InGame_To_InGameResult_With_EndOfGame();
	void Initialize_InGamePause_To_InGame_With_ResumeGame();
	void Initialize_InGamePause_To_InGame_With_RestartGame();
	void Initialize_InGamePause_To_MainMenu_With_ToMainMenu();
	void Initialize_InGamePause_To_Exit_With_ExitGame();
	void Initialize_InGameResult_To_InGame_With_RestartGame();
	void Initialize_InGameResult_To_MainMenu_With_ToMainMenu();
	void Initialize_InGameResult_To_Exit_With_ExitGame();

public:
	UApplicationStateManager();

	virtual void Init() override;
	virtual void Prepare() override;
	virtual void Release() override;
	virtual void Destroy() override;

	bool TriggerStateTransition(EStateTrigger trigger);
	void UpdateState();
};
