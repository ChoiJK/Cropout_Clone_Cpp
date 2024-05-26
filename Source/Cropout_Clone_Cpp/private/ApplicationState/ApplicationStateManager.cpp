// Fill out your copyright notice in the Description page of Project Settings.

#include "ApplicationState/ApplicationStateManager.h"

#include "ApplicationState/DependentProcessQueue.h"
#include "ApplicationState/StateTransitionProcessor.h"
#include "ApplicationState/StateUpdater.h"
#include "ApplicationState/StateProcess/StateProcessFactory.h"
#include "ApplicationState/StateProcess/StateProcess_ClearSaveGame.h"
#include "ApplicationState/StateProcess/StateProcess_ClearSpawner.h"
#include "ApplicationState/StateProcess/StateProcess_CloseMenuWidget.h"
#include "ApplicationState/StateProcess/StateProcess_Initialize.h"
#include "ApplicationState/StateProcess/StateProcess_OpenLevel.h"
#include "ApplicationState/StateProcess/StateProcess_OpenMenuWidget.h"
#include "ApplicationState/StateProcess/StateProcess_SaveGame.h"
#include "ApplicationState/StateProcess/StateProcess_SetGameContinue.h"
#include "ApplicationState/StateProcess/StateProcess_SetGamePausedMode.h"
#include "ApplicationState/StateProcess/StateProcess_SetInputMode.h"
#include "ApplicationState/StateProcess/StateProcess_SetResultMenuWidgetText.h"


UApplicationStateManager::UApplicationStateManager(): CurrentState(ECropoutState::None)
{
}

void UApplicationStateManager::Init()
{
	Super::Init();

	SharedData = NewObject<UStateProcessSharedData>();

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
}

ECropoutState UApplicationStateManager::GetCurrentState() const
{
	return CurrentState;
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
	UDependentProcessQueue* InitializeProcessQueue = NewObject<UDependentProcessQueue>();
	InitializeProcessQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_Initialize());

	UStateTransitionProcessor* None_To_Initialize_Processor = NewObject<UStateTransitionProcessor>();
	None_To_Initialize_Processor->AddTransitionProcess(InitializeProcessQueue);
	None_To_Initialize_Processor->SetOnAllProcessesCompleted([this](void)
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
	UDependentProcessQueue* ToSplashQueue = NewObject<UDependentProcessQueue>();
	ToSplashQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("Splash"));

	UStateTransitionProcessor* Initialize_To_Splash_Processor = NewObject<UStateTransitionProcessor>();
	Initialize_To_Splash_Processor->AddTransitionProcess(ToSplashQueue);

	AddTransitionProcessors(ECropoutState::Initialize, FStateTransition(EStateTrigger::InitializeSuccess,
		ECropoutState::Splash, Initialize_To_Splash_Processor));
}

void UApplicationStateManager::Initialize_Splash_To_MainMenu_With_SplashComplete()
{
	// Splash -> MainMenu : SplashComplete
	// Splash화면이 끝나고 MainMenu화면으로 넘어갈 때 발생
	UDependentProcessQueue* ToMainMenuQueue = NewObject<UDependentProcessQueue>();
	ToMainMenuQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("MainMenu"));

	UStateTransitionProcessor* Splash_To_MainMenu_Processor = NewObject<UStateTransitionProcessor>();
	Splash_To_MainMenu_Processor->AddTransitionProcess(ToMainMenuQueue);
	AddTransitionProcessors(ECropoutState::Splash, FStateTransition(EStateTrigger::SplashComplete,
		ECropoutState::MainMenu, Splash_To_MainMenu_Processor));
}

void UApplicationStateManager::Initialize_MainMenu_To_InGame_With_NewGame()
{
	// MainMenu -> InGame : NewGame
	// MainMenu에서 NewGame버튼을 눌렀을 때 발생
	UDependentProcessQueue* ToInGameQueue = NewObject<UDependentProcessQueue>();
	ToInGameQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSpawner());
	ToInGameQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSaveGame());
	ToInGameQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("InGame"));

	UStateTransitionProcessor* MainMenu_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	MainMenu_To_InGame_Processor->AddTransitionProcess(ToInGameQueue);
	AddTransitionProcessors(ECropoutState::MainMenu, FStateTransition(EStateTrigger::NewGame,
		ECropoutState::InGame, MainMenu_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_MainMenu_To_InGame_With_LoadGame()
{
	// MainMenu -> InGame : LoadGame
	// MainMenu에서 LoadGame버튼을 눌렀을 때 발생
	UDependentProcessQueue* ToInGameQueue = NewObject<UDependentProcessQueue>();
	ToInGameQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSpawner());
	ToInGameQueue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("InGame"));

	UStateTransitionProcessor* MainMenu_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	MainMenu_To_InGame_Processor->AddTransitionProcess(ToInGameQueue);
	AddTransitionProcessors(ECropoutState::MainMenu, FStateTransition(EStateTrigger::LoadGame,
		ECropoutState::InGame, MainMenu_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_MainMenu_To_Exit_With_ExitGame()
{
	// MainMenu -> Exit : ExitGame
}

void UApplicationStateManager::Initialize_InGame_To_InGamePause_With_PauseGame()
{
	// InGame -> InGamePause : PauseGame
	// InGame에서 Pause버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGame_To_InGamePause_Queue = NewObject<UDependentProcessQueue>();
	InGame_To_InGamePause_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGamePausedMode(true));
	InGame_To_InGamePause_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_OpenMenuWidget(EWidgetType::PauseMenu));
	InGame_To_InGamePause_Queue->
		AddDependentProcess(StateProcessFactory::Create_StateProcess_SetInputMode(true, false));

	UStateTransitionProcessor* InGame_To_InGamePause_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGamePause_Processor->AddTransitionProcess(InGame_To_InGamePause_Queue);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::PauseGame,
		ECropoutState::InGamePause, InGame_To_InGamePause_Processor));
}

void UApplicationStateManager::Initialize_InGame_To_InGameResult_With_WinGame()
{
	// InGame -> InGameResult : WinGame
	// 게임 승리조건을 만족했을 때 발생 (EBuildingType::VictoryMonument가 지어졌을 때)
	UDependentProcessQueue* InGame_To_InGameResult_Queue = NewObject<UDependentProcessQueue>();
	InGame_To_InGameResult_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGamePausedMode(true));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_OpenMenuWidget(EWidgetType::ResultMenu));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_SetResultMenuWidgetText("Congratulations! You built the monument!"));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_SetInputMode(true, false));

	UStateTransitionProcessor* InGame_To_InGameResult_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGameResult_Processor->AddTransitionProcess(InGame_To_InGameResult_Queue);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::WinGame,
		ECropoutState::InGameResult, InGame_To_InGameResult_Processor));
}

void UApplicationStateManager::Initialize_InGame_To_InGameResult_With_LoseGame()
{
	// InGame -> InGameResult : LoseGame
	// 게임 패배조건을 만족했을 때 발생 (식량이 0보다 작아지는 경우)
	UDependentProcessQueue* InGame_To_InGameResult_Queue = NewObject<UDependentProcessQueue>();
	InGame_To_InGameResult_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGamePausedMode(true));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_OpenMenuWidget(EWidgetType::ResultMenu));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_SetResultMenuWidgetText("Oh no! You Ran out of food."));
	InGame_To_InGameResult_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_SetInputMode(true, false));

	UStateTransitionProcessor* InGame_To_InGameResult_Processor = NewObject<UStateTransitionProcessor>();
	InGame_To_InGameResult_Processor->AddTransitionProcess(InGame_To_InGameResult_Queue);
	AddTransitionProcessors(ECropoutState::InGame, FStateTransition(EStateTrigger::LoseGame,
		ECropoutState::InGameResult, InGame_To_InGameResult_Processor));
}


void UApplicationStateManager::Initialize_InGamePause_To_InGame_With_ResumeGame()
{
	// InGamePause -> InGame : ResumeGame
	// InGamePause메뉴에서 Resume버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGamePause_To_InGame_Queue = NewObject<UDependentProcessQueue>();
	InGamePause_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_CloseMenuWidget());
	InGamePause_To_InGame_Queue->AddDependentProcess(
		StateProcessFactory::Create_StateProcess_SetInputMode(true, true));
	InGamePause_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGamePausedMode(false));

	UStateTransitionProcessor* InGamePause_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGamePause_To_InGame_Processor->AddTransitionProcess(InGamePause_To_InGame_Queue);
	AddTransitionProcessors(ECropoutState::InGamePause, FStateTransition(EStateTrigger::ResumeGame,
		ECropoutState::InGame, InGamePause_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGamePause_To_InGame_With_RestartGame()
{
	// InGamePause -> InGame : RestartGame
	// InGamePause메뉴에서 Restart버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGamePause_To_InGame_Queue = NewObject<UDependentProcessQueue>();
	InGamePause_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSpawner());
	InGamePause_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSaveGame());
	InGamePause_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("InGame"));

	UStateTransitionProcessor* InGamePause_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGamePause_To_InGame_Processor->AddTransitionProcess(InGamePause_To_InGame_Queue);
	AddTransitionProcessors(ECropoutState::InGamePause, FStateTransition(EStateTrigger::RestartGame,
		ECropoutState::InGame, InGamePause_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGamePause_To_MainMenu_With_ToMainMenu()
{
	// InGamePause -> MainMenu : ToMainMenu
	// InGamePause메뉴에서 MainMenu버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGamePause_To_MainMenu_Queue = NewObject<UDependentProcessQueue>();
	InGamePause_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_CloseMenuWidget());
	InGamePause_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SaveGame());
	InGamePause_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("MainMenu"));

	UStateTransitionProcessor* InGamePause_To_MainMenu_Processor = NewObject<UStateTransitionProcessor>();
	InGamePause_To_MainMenu_Processor->AddTransitionProcess(InGamePause_To_MainMenu_Queue);
	AddTransitionProcessors(ECropoutState::InGamePause, FStateTransition(EStateTrigger::ToMainMenu,
		ECropoutState::MainMenu, InGamePause_To_MainMenu_Processor));
}

void UApplicationStateManager::Initialize_InGamePause_To_Exit_With_ExitGame()
{
	// InGamePause -> Exit : ExitGame
}

void UApplicationStateManager::Initialize_InGameResult_To_InGame_With_ResumeGame()
{
	// InGameResult -> InGame : ResumeGame
	// ResultMenuWidget에서 Continue버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGameResult_To_InGame_Queue = NewObject<UDependentProcessQueue>();
	InGameResult_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_CloseMenuWidget());
	InGameResult_To_InGame_Queue->
		AddDependentProcess(StateProcessFactory::Create_StateProcess_SetInputMode(true, true));
	InGameResult_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGameContinue());
	InGameResult_To_InGame_Queue->
		AddDependentProcess(StateProcessFactory::Create_StateProcess_SetGamePausedMode(false));

	UStateTransitionProcessor* InGameResult_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGameResult_To_InGame_Processor->AddTransitionProcess(InGameResult_To_InGame_Queue);
	AddTransitionProcessors(ECropoutState::InGameResult, FStateTransition(EStateTrigger::ResumeGame,
		ECropoutState::InGame, InGameResult_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGameResult_To_InGame_With_RestartGame()
{
	// InGameResult -> InGame : RestartGame
	// ResultMenuWidget에서 Restart버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGameResult_To_InGame_Queue = NewObject<UDependentProcessQueue>();
	InGameResult_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSpawner());
	InGameResult_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSaveGame());
	InGameResult_To_InGame_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("InGame"));

	UStateTransitionProcessor* InGameResult_To_InGame_Processor = NewObject<UStateTransitionProcessor>();
	InGameResult_To_InGame_Processor->AddTransitionProcess(InGameResult_To_InGame_Queue);
	AddTransitionProcessors(ECropoutState::InGameResult, FStateTransition(EStateTrigger::RestartGame,
		ECropoutState::InGame, InGameResult_To_InGame_Processor));
}

void UApplicationStateManager::Initialize_InGameResult_To_MainMenu_With_ToMainMenu()
{
	// InGameResult -> MainMenu : ToMainMenu
	// ResultMenuWidget에서 MainMenu버튼을 눌렀을 때 발생
	UDependentProcessQueue* InGameResult_To_MainMenu_Queue = NewObject<UDependentProcessQueue>();
	InGameResult_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_CloseMenuWidget());
	InGameResult_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_ClearSaveGame());
	InGameResult_To_MainMenu_Queue->AddDependentProcess(StateProcessFactory::Create_StateProcess_OpenLevel("MainMenu"));

	UStateTransitionProcessor* InGameResult_To_MainMenu_Processor = NewObject<UStateTransitionProcessor>();
	InGameResult_To_MainMenu_Processor->AddTransitionProcess(InGameResult_To_MainMenu_Queue);
	AddTransitionProcessors(ECropoutState::InGameResult, FStateTransition(EStateTrigger::ToMainMenu,
		ECropoutState::MainMenu, InGameResult_To_MainMenu_Processor));
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
				TransitionProcessor = stateTransition.TransitionProcessor;
				TransitionProcessor->Init();
				return true;
			}
		}
	}

	TransitionProcessor = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("TriggerStateTransition : CurrentState(%d) Trigger(%d) is not valid"),
		(int)CurrentState, (int)trigger);

	return false;
}

void UApplicationStateManager::UpdateState()
{
	if(TransitionProcessor)
	{
		if(TransitionProcessor->IsSuccess() == false)
		{
			TransitionProcessor->Process(SharedData);
		}
		else
		{
			TransitionProcessor = nullptr;
		}
	}

	if(TransitionProcessor && TransitionProcessor->IsSuccess() == false)
	{
		// 처리중인 상태가 있으면 처리를 계속함
		return;
	}

	if(StateUpdaters.Contains(CurrentState))
	{
		StateUpdaters[CurrentState]->DoUpdate();
	}
}
