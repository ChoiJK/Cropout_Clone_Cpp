// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Manager/Base/LevelManager.h"
#include "CropoutLevelScriptBase.generated.h"

class ULevelManager;

UCLASS()
class CROPOUT_CLONE_CPP_API ACropoutLevelScriptBase : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACropoutLevelScriptBase();

	TMap<UClass*, ULevelManager*> LevelManagerMap;

	template <typename Type>
	Type* GetLevelManager()
	{
		static_assert(std::is_base_of_v<ULevelManager, Type>, "Type must be derived from ULevelManager");
		if(LevelManagerMap.Contains(Type::StaticClass()) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("LevelManager not found: %s"), *Type::StaticClass()->GetName());
			return nullptr;
		}

		return Cast<Type>(LevelManagerMap[Type::StaticClass()]);
	}

private:
	void InitializeLevelManagers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
};
