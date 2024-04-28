// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ApplicationState/ApplicationStateManager.h"

#include "Core/ApplicationState/StateStaticMethods.h"
#include "Core/ApplicationState/StateTransitionProcessor.h"
#include "Core/ApplicationState/StateUpdater.h"


UApplicationStateManager::UApplicationStateManager(): CurrentState(ECropoutState::InGame)
{
}

void UApplicationStateManager::Init()
{
	Super::Init();

	Initialize_InGame_To_InGamePause_With_PauseGame();
	Initialize_InGamePause_To_InGame_With_ResumeGame();
}

void UApplicationStateManager::Prepare()
{
	Super::Prepare();
}

void UApplicationStateManager::Release()
{
	Super::Release();
}

void UApplicationStateManager::Destroy()
{
	Super::Destroy();
}


void UApplicationStateManager::InitializeStateUpdaters()
{
}

void UApplicationStateManager::AddTransitionProcessors(ECropoutState state, FStateTransition transitions)
{
	if(TransitionProcessors.Contains(state) == false)
	{
		TransitionProcessors.Add(state, FStateTransitions());
	}

	TransitionProcessors[state].Transitions.Add(transitions);
}


void UApplicationStateManager::Initialize_None_To_Initialize_With_DoInitialize()
{
	// None -> Initialize : DoInitialize
}

void UApplicationStateManager::Initialize_Initialize_To_Splash_With_InitializeSuccess()
{
	// Initialize -> Splash : InitializeSuccess
}

void UApplicationStateManager::Initialize_Splash_To_MainMenu_With_SplashComplete()
{
	// Splash -> MainMenu : SplashComplete
}

void UApplicationStateManager::Initialize_MainMenu_To_InGame_With_NewGame()
{
	// MainMenu -> InGame : NewGame
}

void UApplicationStateManager::Initialize_MainMenu_To_InGame_With_LoadGame()
{
	// MainMenu -> InGame : LoadGame
}

void UApplicationStateManager::Initialize_MainMenu_To_Exit_With_ExitGame()
{
	// MainMenu -> Exit : ExitGame
}

void UApplicationStateManager::Initialize_InGame_To_InGamePause_With_PauseGame()
{
	// InGame -> InGamePause : PauseGame
	// InGame에서 Pause버튼을 눌렀을 때 발생
	UStateTransitionProcessor* InGame_To_InGamePause_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGamePause_Processor->AddClearPreviousStateProcess(FStateStaticMethods::SetGamePaused);
	InGame_To_InGamePause_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenPauseMenuWidget);
	InGame_To_InGamePause_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetOnlyUIInputMode);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::PauseGame,
		ECropoutState::InGamePause, InGame_To_InGamePause_Processor));
}

void UApplicationStateManager::Initialize_InGame_To_InGameResult_With_EndOfGame()
{
	// InGame -> InGameResult : EndOfGame
}

void UApplicationStateManager::Initialize_InGamePause_To_InGame_With_ResumeGame()
{
	// InGamePause -> InGame : ResumeGame
	// InGamePause메뉴에서 Resume버튼을 눌렀을 때 발생
	UStateTransitionProcessor* InGamePause_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGamePause_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::ClosePauseMenuWidget);
	InGamePause_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::SetGameAndUIInputMode);
	InGamePause_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetGameUnpaused);
	AddTransitionProcessors(ECropoutState::InGamePause, FStateTransition(EStateTrigger::ResumeGame,
		ECropoutState::InGame, InGamePause_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGamePause_To_InGame_With_RestartGame()
{
	// InGamePause -> InGame : RestartGame
}

void UApplicationStateManager::Initialize_InGamePause_To_MainMenu_With_ToMainMenu()
{
	// InGamePause -> MainMenu : ToMainMenu
}

void UApplicationStateManager::Initialize_InGamePause_To_Exit_With_ExitGame()
{
	// InGamePause -> Exit : ExitGame
}

void UApplicationStateManager::Initialize_InGameResult_To_InGame_With_RestartGame()
{
	// InGameResult -> InGame : RestartGame
}

void UApplicationStateManager::Initialize_InGameResult_To_MainMenu_With_ToMainMenu()
{
	// InGameResult -> MainMenu : ToMainMenu
}

void UApplicationStateManager::Initialize_InGameResult_To_Exit_With_ExitGame()
{
	// InGameResult -> Exit : ExitGame
}


bool UApplicationStateManager::TriggerStateTransition(EStateTrigger trigger)
{
	if(TransitionProcessors.Contains(CurrentState))
	{
		for(FStateTransition& stateTransition : TransitionProcessors[CurrentState].Transitions)
		{
			if(stateTransition.Trigger == trigger)
			{
				CurrentState = stateTransition.ToState;
				// @Todo :  DoClearPreviousState, DoPrepareNextState를 Async하게 처리할 수 있도록 수정
				// Clear를 Async로 처리하고 모든 Clear가 끝나면 Prepare를 Async로 처리하도록
				// (Tick이랑 꼬일것같기도한데...)
				stateTransition.TransitionProcessor->DoClearPreviousState();
				stateTransition.TransitionProcessor->DoPrepareNextState();
				return true;
			}
		}
	}
	return false;
}

void UApplicationStateManager::UpdateState()
{
	if(StateUpdaters.Contains(CurrentState))
	{
		StateUpdaters[CurrentState]->DoUpdate();
	}
}
