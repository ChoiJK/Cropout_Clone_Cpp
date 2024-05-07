// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeIdle.generated.h"

UCLASS()
class CROPOUT_CLONE_CPP_API UCameraShakeIdle : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	UCameraShakeIdle(const FObjectInitializer& ObjInit);
};
