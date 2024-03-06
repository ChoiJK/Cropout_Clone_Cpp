// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CropoutPlayerController.generated.h"


enum class EInputType
{
	Unknown,
	KeyMouse,
	GamePad,
	Touch
};

class FGlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayerController : public APlayerController
{
	GENERATED_BODY()

	TWeakPtr<FGlobalEventDispatcher> GlobalEventDispatcher;
	EInputType CurrentInputType = EInputType::Unknown;

	void OnKeyDetect(const FKey Key);
	void OnTouchDetect(const FKey Key);
	void OnMouseMove(const float Value);
	void SwitchInputType(EInputType inputType);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	EInputType GetCurrentInputType() { return CurrentInputType; }
};
