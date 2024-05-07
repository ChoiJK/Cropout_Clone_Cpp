// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CropoutPlayerController.h"

#include "Core/CropoutGameInstance.h"
#include "Util/CoordinateUtils.h"

ACropoutPlayerController::ACropoutPlayerController()
{
	CurrentInputType = std::make_shared<SimpleRxReactiveProperty<EInputType>>(EInputType::Unknown);
}

void ACropoutPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CoordinateUtils::SetPlayerController(this);

	SetInputMode(FInputModeGameAndUI());

	auto gameInstance = UCropoutGameInstance::GetInstance();
	gameInstance->SetCurrentPlayerController(this);
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

void ACropoutPlayerController::Destroyed()
{
	Super::Destroyed();

	CoordinateUtils::SetPlayerController(nullptr);
}

std::shared_ptr<SimpleRxReactiveProperty<EInputType>> ACropoutPlayerController::GetCurrentInputTypeRP()
{
	return CurrentInputType;
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
	if(CurrentInputType->GetValue() == inputType)
	{
		return;
	}

	CurrentInputType->SetValue(inputType);
}
