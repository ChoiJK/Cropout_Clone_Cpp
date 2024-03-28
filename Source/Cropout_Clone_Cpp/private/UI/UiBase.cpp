// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UiBase.h"

#include "Widgets/CommonactivatableWidgetContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/CropoutPlayerController.h"


void UUiBase::NativeConstruct()
{
	Super::NativeConstruct();
	PromptStack->OnDisplayedWidgetChanged().AddUObject(this, &UUiBase::OnPromptStackChanged);
}

UCommonActivatableWidget* UUiBase::PushMenuClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return MainStack->AddWidget(widgetClass);
}

UCommonActivatableWidget* UUiBase::PushPromptClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return PromptStack->AddWidget(widgetClass);
}

UCommonActivatableWidget* UUiBase::PushBottomClass(TSubclassOf<UCommonActivatableWidget> widgetClass)
{
	return BottomStack->AddWidget(widgetClass);
}

void UUiBase::OnPromptStackChanged(UCommonActivatableWidget* CommonActivatableWidget)
{
	bool isUIInput = true;

	if(CommonActivatableWidget == nullptr)
	{
		isUIInput = false;
	}

	if(OnlyUIInput != isUIInput)
	{
		ACropoutPlayerController* playerController = Cast<ACropoutPlayerController>(
			UGameplayStatics::GetPlayerController(this, 0));

		if(isUIInput)
		{
			UGameplayStatics::SetGamePaused(this, true);
			playerController->SetInputMode(FInputModeUIOnly());
			playerController->bShowMouseCursor = true;
		}
		else
		{
			UGameplayStatics::SetGamePaused(this, false);
			playerController->SetInputMode(FInputModeGameAndUI());
		}

		OnlyUIInput = isUIInput;
	}
}
