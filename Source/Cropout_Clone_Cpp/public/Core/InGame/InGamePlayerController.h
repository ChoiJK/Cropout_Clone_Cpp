// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleRx/SimpleRxReactiveProperty.h"
#include "InGamePlayerController.generated.h"

enum class EInputType
{
	Unknown,
	KeyMouse,
	GamePad,
	Touch
};

UCLASS()
class CROPOUT_CLONE_CPP_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	AInGamePlayerController();

	void OnKeyDetect(const FKey Key);
	void OnTouchDetect(const FKey Key);

private:
	void OnMouseMove(const float Value);
	void SwitchInputType(EInputType inputType);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Destroyed() override;

public:
	std::shared_ptr<SimpleRxReactiveProperty<EInputType>> CurrentInputType;

	std::shared_ptr<SimpleRxReactiveProperty<EInputType>> GetCurrentInputTypeRP();
	EInputType GetCurrentInputType() const { return CurrentInputType->GetValue(); }
};
