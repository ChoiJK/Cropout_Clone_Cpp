// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Global/GlobalSharedVariable.h"
#include "IslandBaseGameMode.generated.h"

class UCropoutGameInstance;
class FGlobalEventDispatcher;
class UGlobalSharedVariable;
class ASpawner;

UCLASS()
class CROPOUT_CLONE_CPP_API AIslandBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIslandBaseGameMode();

private:
	UPROPERTY()
	ASpawner* SpawnerRef;

protected:
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;
	UGlobalSharedVariable* GlobalSharedVariable;

	FDelegateHandle IslandGenCompleteHandle;

	virtual void OnIslandGenCompleteInternal();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void OnIslandGenComplete();

	UCropoutGameInstance* GetCropoutGameInstance();
	ASpawner* GetSpawner();
};
