#include "StateProcessFactory.h"

#include "StateProcess_ClearSaveGame.h"
#include "StateProcess_ClearSpawner.h"
#include "StateProcess_CloseMenuWidget.h"
#include "StateProcess_Initialize.h"
#include "StateProcess_OpenLevel.h"
#include "StateProcess_OpenMenuWidget.h"
#include "StateProcess_SaveGame.h"
#include "StateProcess_SetGameContinue.h"
#include "StateProcess_SetGamePausedMode.h"
#include "StateProcess_SetInputMode.h"
#include "StateProcess_SetResultMenuWidgetText.h"

UStateProcess_Initialize* StateProcessFactory::Create_StateProcess_Initialize()
{
	return NewObject<UStateProcess_Initialize>();
}

UStateProcess_OpenLevel* StateProcessFactory::Create_StateProcess_OpenLevel(FString levelName)
{
	UStateProcess_OpenLevel* level = NewObject<UStateProcess_OpenLevel>();
	level->SetLevelName(levelName);
	return level;
}

UStateProcess_SaveGame* StateProcessFactory::Create_StateProcess_SaveGame()
{
	return NewObject<UStateProcess_SaveGame>();
}

UStateProcess_ClearSaveGame* StateProcessFactory::Create_StateProcess_ClearSaveGame()
{
	return NewObject<UStateProcess_ClearSaveGame>();
}

UStateProcess_ClearSpawner* StateProcessFactory::Create_StateProcess_ClearSpawner()
{
	return NewObject<UStateProcess_ClearSpawner>();
}

UStateProcess_SetGameContinue* StateProcessFactory::Create_StateProcess_SetGameContinue()
{
	return NewObject<UStateProcess_SetGameContinue>();
}

UStateProcess_SetGamePausedMode* StateProcessFactory::Create_StateProcess_SetGamePausedMode(bool bPaused)
{
	UStateProcess_SetGamePausedMode* mode = NewObject<UStateProcess_SetGamePausedMode>();
	mode->SetPaused(bPaused);
	return mode;
}

UStateProcess_SetInputMode* StateProcessFactory::Create_StateProcess_SetInputMode(bool uiInputMode, bool gameInputMode)
{
	UStateProcess_SetInputMode* inputMode = NewObject<UStateProcess_SetInputMode>();
	inputMode->SetInputMode(uiInputMode, gameInputMode);
	return inputMode;
}

UStateProcess_OpenMenuWidget* StateProcessFactory::Create_StateProcess_OpenMenuWidget(EWidgetType widgetType)
{
	UStateProcess_OpenMenuWidget* menuWidget = NewObject<UStateProcess_OpenMenuWidget>();
	menuWidget->SetWidgetType(widgetType);
	return menuWidget;
}

UStateProcess_CloseMenuWidget* StateProcessFactory::Create_StateProcess_CloseMenuWidget()
{
	return NewObject<UStateProcess_CloseMenuWidget>();
}

UStateProcess_SetResultMenuWidgetText* StateProcessFactory::Create_StateProcess_SetResultMenuWidgetText(FString string)
{
	UStateProcess_SetResultMenuWidgetText* widgetText = NewObject<UStateProcess_SetResultMenuWidgetText>();
	widgetText->SetMenuText(string);
	return widgetText;
}
