#pragma once

class UCommonActivatableWidget;

static class FStateStaticMethods
{
	static UCommonActivatableWidget* PauseMenuWidget;

	static UCommonActivatableWidget* ResultMenuWidget;

public:
	static void Initialize();

	static void OpenSplashLevel();
	static void OpenMainMenuLevel();
	static void OpenInGameLevel();

	static void SaveGame();
	static void ClearSaveGame();
	static void ClearSpawner();

	static void SetGameContinue();

	static void SetGamePaused();
	static void SetGameUnpaused();

	static void SetOnlyUIInputMode();
	static void SetGameAndUIInputMode();

	static void OpenPauseMenuWidget();
	static void ClosePauseMenuWidget();

	static void OpenResultMenuWidget();
	static void CloseResultMenuWidget();
	static void SetResultMenuWidgetText(FString string);
};
