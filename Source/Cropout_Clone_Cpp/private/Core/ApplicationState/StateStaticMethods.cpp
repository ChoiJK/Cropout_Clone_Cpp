#include "Core/ApplicationState/StateStaticMethods.h"

#include "Blueprint/UserWidget.h"

#include "Core/CropoutGameInstance.h"
#include "Core/CropoutGameMode.h"
#include "Core/CropoutPlayerController.h"
#include "Manager/TableManager.h"
#include "UI/UiManager.h"
#include "UI/Panel/ResultMenuWidget.h"

UCommonActivatableWidget* FStateStaticMethods::PauseMenuWidget = nullptr;
UCommonActivatableWidget* FStateStaticMethods::ResultMenuWidget = nullptr;

void FStateStaticMethods::SetGameContinue()
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	ACropoutGameMode* gameMode = gameInstance->GetCurrentGameMode();
	gameMode->SetIsContinueGame(true);
}

void FStateStaticMethods::SetGamePaused()
{
	const UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	ACropoutPlayerController* const PC = gameInstance->GetCurrentPlayerController();
	PC->SetPause(true);
}

void FStateStaticMethods::SetGameUnpaused()
{
	const UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	ACropoutPlayerController* const PC = gameInstance->GetCurrentPlayerController();
	PC->SetPause(false);
}

void FStateStaticMethods::SetOnlyUIInputMode()
{
	const UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	ACropoutPlayerController* const PC = gameInstance->GetCurrentPlayerController();
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void FStateStaticMethods::SetGameAndUIInputMode()
{
	const UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	ACropoutPlayerController* const PC = gameInstance->GetCurrentPlayerController();
	PC->SetInputMode(FInputModeGameAndUI());
	PC->bShowMouseCursor = false;
}

void FStateStaticMethods::OpenPauseMenuWidget()
{
	if(PauseMenuWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseMenuWidget is already opened"));
		return;
	}

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UTableManager* TableManager = gameInstance->GetSystemManager<UTableManager>();
	UUiManager* UiManager = gameInstance->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* pauseMenu = TableManager->GetWidgetClass(EWidgetType::PauseMenu);
	PauseMenuWidget = UiManager->GetUIBase()->PushPromptClass(pauseMenu->Get());
}

void FStateStaticMethods::ClosePauseMenuWidget()
{
	if(PauseMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseMenuWidget is already closed"));
		return;
	}

	if(PauseMenuWidget != nullptr)
	{
		PauseMenuWidget->DeactivateWidget();
		PauseMenuWidget = nullptr;
	}
}

void FStateStaticMethods::OpenResultMenuWidget()
{
	if(ResultMenuWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResultMenuWidget is already opened"));
		return;
	}

	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UTableManager* TableManager = gameInstance->GetSystemManager<UTableManager>();
	UUiManager* UiManager = gameInstance->GetLevelManager<UUiManager>();

	TSubclassOf<UUserWidget>* resultMenu = TableManager->GetWidgetClass(EWidgetType::ResultMenu);
	ResultMenuWidget = UiManager->GetUIBase()->PushPromptClass(resultMenu->Get());
}

void FStateStaticMethods::CloseResultMenuWidget()
{
	if(ResultMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResultMenuWidget is already closed"));
		return;
	}

	if(ResultMenuWidget != nullptr)
	{
		ResultMenuWidget->DeactivateWidget();
		ResultMenuWidget = nullptr;
	}
}

void FStateStaticMethods::SetResultMenuWidgetText(FString string)
{
	if(ResultMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResultMenuWidget is not opened"));
		return;
	}

	const UResultMenuWidget* resultMenuWidget = Cast<UResultMenuWidget>(ResultMenuWidget);
	resultMenuWidget->SetResultText(FText::FromString(string));
}
