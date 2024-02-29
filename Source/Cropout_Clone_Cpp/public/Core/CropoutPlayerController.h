// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CropoutPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
