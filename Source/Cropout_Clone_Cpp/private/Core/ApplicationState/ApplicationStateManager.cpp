// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ApplicationState/ApplicationStateManager.h"

#include "Core/ApplicationState/StateStaticMethods.h"
#include "Core/ApplicationState/StateTransitionProcessor.h"
#include "Core/ApplicationState/StateUpdater.h"


UApplicationStateManager::UApplicationStateManager(): CurrentState(ECropoutState::None)
{
}

void UApplicationStateManager::Init()
{
	Super::Init();

	Initialize_None_To_Initialize_With_DoInitialize();
	Initialize_Initialize_To_Splash_With_InitializeSuccess();
	Initialize_Splash_To_MainMenu_With_SplashComplete();
	Initialize_MainMenu_To_InGame_With_NewGame();
	Initialize_MainMenu_To_InGame_With_LoadGame();
	Initialize_MainMenu_To_Exit_With_ExitGame();
	Initialize_InGame_To_InGamePause_With_PauseGame();
	Initialize_InGame_To_InGameResult_With_WinGame();
	Initialize_InGame_To_InGameResult_With_LoseGame();
	Initialize_InGamePause_To_InGame_With_ResumeGame();
	Initialize_InGamePause_To_InGame_With_RestartGame();
	Initialize_InGamePause_To_MainMenu_With_ToMainMenu();
	Initialize_InGamePause_To_Exit_With_ExitGame();
	Initialize_InGameResult_To_InGame_With_ResumeGame();
	Initialize_InGameResult_To_InGame_With_RestartGame();
	Initialize_InGameResult_To_MainMenu_With_ToMainMenu();
}

void UApplicationStateManager::Prepare()
{
	Super::Prepare();

	TriggerStateTransition(EStateTrigger::DoInitialize);
}

void UApplicationStateManager::Release()
{
	Super::Release();
}

void UApplicationStateManager::Destroy()
{
	Super::Destroy();
}

void UApplicationStateManager::OnLevelLoaded()
{
	Super::OnLevelLoaded();

	FStateStaticMethods::Initialize();
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
	// 게임이 시작될 때 발생
	UStateTransitionProcessor* None_To_Initialize_Processor = NewObject<UStateTransitionProcessor>();
	//None_To_Initialize_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenInGameLevel);
	None_To_Initialize_Processor->AddPrepareNextStateProcess([this](void)
	{
		TriggerStateTransition(EStateTrigger::InitializeSuccess);
	});
	AddTransitionProcessors(ECropoutState::None, FStateTransition(EStateTrigger::DoInitialize,
		ECropoutState::Initialize, None_To_Initialize_Processor));
}

void UApplicationStateManager::Initialize_Initialize_To_Splash_With_InitializeSuccess()
{
	// Initialize -> Splash : InitializeSuccess
	// 게임이 시작되고 초기화가 완료되었을 때 발생
	UStateTransitionProcessor* Initialize_To_Splash_Processor = NewObject<UStateTransitionProcessor>();
	Initialize_To_Splash_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenSplashLevel);
	AddTransitionProcessors(ECropoutState::Initialize, FStateTransition(EStateTrigger::InitializeSuccess,
		ECropoutState::Splash, Initialize_To_Splash_Processor));
}

void UApplicationStateManager::Initialize_Splash_To_MainMenu_With_SplashComplete()
{
	// Splash -> MainMenu : SplashComplete
	// Splash화면이 끝나고 MainMenu화면으로 넘어갈 때 발생
	UStateTransitionProcessor* Splash_To_MainMenu_Processor = NewObject<UStateTransitionProcessor>();
	Splash_To_MainMenu_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenMainMenuLevel);
	AddTransitionProcessors(ECropoutState::Splash, FStateTransition(EStateTrigger::SplashComplete,
		ECropoutState::MainMenu, Splash_To_MainMenu_Processor));
}

void UApplicationStateManager::Initialize_MainMenu_To_InGame_With_NewGame()
{
	// MainMenu -> InGame : NewGame
	// MainMenu에서 NewGame버튼을 눌렀을 때 발생
	UStateTransitionProcessor* MainMenu_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	MainMenu_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::ClearSpawner);
	MainMenu_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenInGameLevel);
	AddTransitionProcessors(ECropoutState::MainMenu, FStateTransition(EStateTrigger::NewGame,
		ECropoutState::InGame, MainMenu_To_InGame_Processor));
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

void UApplicationStateManager::Initialize_InGame_To_InGameResult_With_WinGame()
{
	// InGame -> InGameResult : WinGame
	// 게임 승리조건을 만족했을 때 발생 (EBuildingType::VictoryMonument가 지어졌을 때)
	UStateTransitionProcessor* InGame_To_InGameResult_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGameResult_Processor->AddClearPreviousStateProcess(FStateStaticMethods::SetGamePaused);
	InGame_To_InGameResult_Processor->AddPrepareNextStateProcess([](void)
	{
		FStateStaticMethods::OpenResultMenuWidget();
		FStateStaticMethods::SetResultMenuWidgetText("Congratulations! You built the monument!");
	});
	InGame_To_InGameResult_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetOnlyUIInputMode);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::WinGame,
		ECropoutState::InGameResult, InGame_To_InGameResult_Processor));
}

void UApplicationStateManager::Initialize_InGame_To_InGameResult_With_LoseGame()
{
	// InGame -> InGameResult : LoseGame
	// 게임 패배조건을 만족했을 때 발생 (식량이 0보다 작아지는 경우)
	UStateTransitionProcessor* InGame_To_InGameResult_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGameResult_Processor->AddClearPreviousStateProcess(FStateStaticMethods::SetGamePaused);
	InGame_To_InGameResult_Processor->AddPrepareNextStateProcess([](void)
	{
		FStateStaticMethods::OpenResultMenuWidget();
		FStateStaticMethods::SetResultMenuWidgetText("Oh no! You Ran out of food.");
	});
	InGame_To_InGameResult_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetOnlyUIInputMode);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::LoseGame,
		ECropoutState::InGameResult, InGame_To_InGameResult_Processor));
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
	// InGamePause메뉴에서 Restart버튼을 눌렀을 때 발생
	UStateTransitionProcessor* InGamePause_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGamePause_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::ClearSpawner);
	InGamePause_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenInGameLevel);
	AddTransitionProcessors(ECropoutState::InGamePause, FStateTransition(EStateTrigger::RestartGame,
		ECropoutState::InGame, InGamePause_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGamePause_To_MainMenu_With_ToMainMenu()
{
	// InGamePause -> MainMenu : ToMainMenu
}

void UApplicationStateManager::Initialize_InGamePause_To_Exit_With_ExitGame()
{
	// InGamePause -> Exit : ExitGame
}

void UApplicationStateManager::Initialize_InGameResult_To_InGame_With_ResumeGame()
{
	// InGameResult -> InGame : ResumeGame
	// ResultMenuWidget에서 Continue버튼을 눌렀을 때 발생	
	UStateTransitionProcessor* InGameResult_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGameResult_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::CloseResultMenuWidget);
	InGameResult_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::SetGameAndUIInputMode);
	InGameResult_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetGameContinue);
	InGameResult_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::SetGameUnpaused);
	AddTransitionProcessors(ECropoutState::InGameResult, FStateTransition(EStateTrigger::ResumeGame,
		ECropoutState::InGame, InGameResult_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGameResult_To_InGame_With_RestartGame()
{
	// InGameResult -> InGame : RestartGame
	// ResultMenuWidget에서 Restart버튼을 눌렀을 때 발생
	UStateTransitionProcessor* InGameResult_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGameResult_To_InGame_Processor->AddClearPreviousStateProcess(FStateStaticMethods::ClearSpawner);
	InGameResult_To_InGame_Processor->AddPrepareNextStateProcess(FStateStaticMethods::OpenInGameLevel);
	AddTransitionProcessors(ECropoutState::InGameResult, FStateTransition(EStateTrigger::RestartGame,
		ECropoutState::InGame, InGameResult_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGameResult_To_MainMenu_With_ToMainMenu()
{
	// InGameResult -> MainMenu : ToMainMenu
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

	UE_LOG(LogTemp, Warning, TEXT("TriggerStateTransition : CurrentState(%d) Trigger(%d) is not valid"),
		(int)CurrentState, (int)trigger);

	return false;
}

void UApplicationStateManager::UpdateState()
{
	if(StateUpdaters.Contains(CurrentState))
	{
		StateUpdaters[CurrentState]->DoUpdate();
	}
}
