#pragma once

class UCommonActivatableWidget;

static class FStateStaticMethods
{
	static UCommonActivatableWidget* PauseMenuWidget;

public:
	static void SetGamePaused();
	static void SetGameUnpaused();

	static void SetOnlyUIInputMode();
	static void SetGameAndUIInputMode();

	static void OpenPauseMenuWidget();
	static void ClosePauseMenuWidget();
};
