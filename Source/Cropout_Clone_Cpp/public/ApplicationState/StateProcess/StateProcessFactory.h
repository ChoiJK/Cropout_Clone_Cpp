#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Enum/WidgetType.h"

class UStateProcess_OpenLevel;
class UStateProcess_Initialize;

class UStateProcess_SaveGame;
class UStateProcess_ClearSaveGame;
class UStateProcess_ClearSpawner;
class UStateProcess_ClearGrassMoveRT;

class UStateProcess_SetGameContinue;

class UStateProcess_SetGamePausedMode;
class UStateProcess_SetInputMode;

class UStateProcess_OpenMenuWidget;
class UStateProcess_CloseMenuWidget;
class UStateProcess_SetResultMenuWidgetText;

class UStateProcess_ProcessLambda;

class UStateProcess_ExitGame;

static class StateProcessFactory
{
public:
	static UStateProcess_Initialize* Create_StateProcess_Initialize();

	static UStateProcess_OpenLevel* Create_StateProcess_OpenLevel(FString levelName);

	static UStateProcess_SaveGame* Create_StateProcess_SaveGame();
	static UStateProcess_ClearSaveGame* Create_StateProcess_ClearSaveGame();
	static UStateProcess_ClearSpawner* Create_StateProcess_ClearSpawner();
	static UStateProcess_ClearGrassMoveRT* Create_StateProcess_ClearGrassMoveRT();

	static UStateProcess_SetGameContinue* Create_StateProcess_SetGameContinue();

	static UStateProcess_SetGamePausedMode* Create_StateProcess_SetGamePausedMode(bool bPaused);
	static UStateProcess_SetInputMode* Create_StateProcess_SetInputMode(bool uiInputMode, bool gameInputMode);

	static UStateProcess_OpenMenuWidget* Create_StateProcess_OpenMenuWidget(EWidgetType widgetType);
	static UStateProcess_CloseMenuWidget* Create_StateProcess_CloseMenuWidget();
	static UStateProcess_SetResultMenuWidgetText* Create_StateProcess_SetResultMenuWidgetText(FString string);
};
