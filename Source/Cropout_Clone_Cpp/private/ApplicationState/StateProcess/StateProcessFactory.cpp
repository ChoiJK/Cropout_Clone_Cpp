#include "ApplicationState/StateProcess/StateProcessFactory.h"

#include "ApplicationState/StateProcess/StateProcess_ClearGrassMoveRT.h"
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

UStateProcess_ClearGrassMoveRT* StateProcessFactory::Create_StateProcess_ClearGrassMoveRT()
{
	return NewObject<UStateProcess_ClearGrassMoveRT>();
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
