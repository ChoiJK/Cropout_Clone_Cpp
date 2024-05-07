// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/InGame/InGamePlayerController.h"

#include "Core/CropoutGameInstance.h"
#include "Util/CoordinateUtils.h"

AInGamePlayerController::AInGamePlayerController()
{
	CurrentInputType = std::make_shared<SimpleRxReactiveProperty<EInputType>>(EInputType::Unknown);
}

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	CoordinateUtils::SetPlayerController(this);

	SetInputMode(FInputModeGameAndUI());

	auto gameInstance = UCropoutGameInstance::GetInstance();
	gameInstance->SetCurrentPlayerController(this);
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("KeyDetect", IE_Pressed, this,
		&AInGamePlayerController::OnKeyDetect);
	InputComponent->BindAction("TouchDetect", IE_Pressed, this,
		&AInGamePlayerController::OnTouchDetect);
	InputComponent->BindAxis("MouseMove", this,
		&AInGamePlayerController::OnMouseMove);
}

void AInGamePlayerController::OnKeyDetect(const FKey Key)
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

void AInGamePlayerController::OnTouchDetect(const FKey Key)
{
	SwitchInputType(EInputType::Touch);
}

void AInGamePlayerController::Destroyed()
{
	Super::Destroyed();

	CoordinateUtils::SetPlayerController(nullptr);
}

std::shared_ptr<SimpleRxReactiveProperty<EInputType>> AInGamePlayerController::GetCurrentInputTypeRP()
{
	return CurrentInputType;
}

void AInGamePlayerController::OnMouseMove(const float Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	SwitchInputType(EInputType::KeyMouse);
}

void AInGamePlayerController::SwitchInputType(EInputType inputType)
{
	if(CurrentInputType->GetValue() == inputType)
	{
		return;
	}

	CurrentInputType->SetValue(inputType);
}
