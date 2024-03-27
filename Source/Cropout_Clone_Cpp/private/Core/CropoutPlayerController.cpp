// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutPlayerController.h"

#include "Core/CropoutGameInstance.h"
#include "Global/GlobalEventDispatcher.h"

void ACropoutPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//bShowMouseCursor = true;
	//bEnableMouseOverEvents = true;
	//bEnableMotionControls = true;
	//bEnableClickEvents = false;
	//bEnableTouchOverEvents = false;

	//AutoReceiveInput = EAutoReceiveInput::Player0;

	SetInputMode(FInputModeGameAndUI());

	auto gameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	if(gameInstance)
	{
		GlobalEventDispatcher = gameInstance->GetGlobalEventDispatcher();
	}
}

void ACropoutPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("KeyDetect", IE_Pressed, this,
	                           &ACropoutPlayerController::OnKeyDetect);
	InputComponent->BindAction("TouchDetect", IE_Pressed, this,
	                           &ACropoutPlayerController::OnTouchDetect);
	InputComponent->BindAxis("MouseMove", this,
	                         &ACropoutPlayerController::OnMouseMove);
}

void ACropoutPlayerController::OnKeyDetect(const FKey Key)
{
	if(Key.IsGamepadKey())
	{
		SwitchInputType(EInputType::GamePad);
	}
	else
	{
		SwitchInputType(EInputType::KeyMouse);
	}
}

void ACropoutPlayerController::OnTouchDetect(const FKey Key)
{
	SwitchInputType(EInputType::Touch);
}

void ACropoutPlayerController::OnMouseMove(const float Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	SwitchInputType(EInputType::KeyMouse);
}

void ACropoutPlayerController::SwitchInputType(EInputType inputType)
{
	if(CurrentInputType == inputType)
	{
		return;
	}

	CurrentInputType = inputType;

	if(TSharedPtr<FGlobalEventDispatcher> shared = GlobalEventDispatcher.Pin())
	{
		shared->Dispatch(EGlobalEventType::ChangedInputType);
	}
}
