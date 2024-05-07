// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMenu/CameraShakeIdle.h"
#include "PerlinNoiseCameraShakePattern.h"

UCameraShakeIdle::UCameraShakeIdle(const FObjectInitializer& ObjInit): Super(ObjInit)
{
	UPerlinNoiseCameraShakePattern* shakePattern = NewObject<UPerlinNoiseCameraShakePattern>();
	shakePattern->LocationAmplitudeMultiplier = 1.0f;
	shakePattern->LocationFrequencyMultiplier = 0.6f;
	shakePattern->X.Amplitude = 100.0f;
	shakePattern->X.Frequency = 0.1f;
	shakePattern->Y.Amplitude = 100.0f;
	shakePattern->Y.Frequency = 0.1f;
	shakePattern->Z.Amplitude = 100.0f;
	shakePattern->Z.Frequency = 0.5f;

	shakePattern->RotationAmplitudeMultiplier = 0.0f;
	shakePattern->RotationFrequencyMultiplier = 1.0f;

	shakePattern->FOV.Amplitude = 0.0f;
	shakePattern->FOV.Frequency = 1.0f;

	shakePattern->Duration = 0.0f;
	shakePattern->BlendInTime = 0.0f;
	shakePattern->BlendOutTime = 0.0f;

	SetRootShakePattern(shakePattern);
}
