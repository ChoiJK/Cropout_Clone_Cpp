// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableBaseActor.h"
#include "Components/TimelineComponent.h"
#include "VisualLogger/VisualLoggerTypes.h"
#include "ResourceBaseActor.generated.h"

UENUM()
enum class EResourceType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
};

inline FString EnumToString(EResourceType Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);
	if(!EnumPtr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetNameStringByIndex(static_cast<uint8>(Value));
}

class UCurveFLoat;
class FGlobalEventDispatcher;

UCLASS()
class CROPOUT_CLONE_CPP_API AResourceBaseActor : public AInteractableBaseActor
{
	GENERATED_BODY()

public:
	AResourceBaseActor();
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;
	FDelegateHandle ScaleUpHandle;
	void ScaleUp();
	bool IsInitResourceScaleCurveSuccess();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FTimeline Timeline;
	void StartTimeline();
	void StopTimeline();

	void TimelineUpdate(float scaleValue) const;
	static void TimelineFinished();

public:
	void DoDeath();
	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* Curve; // timeline

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	EResourceType ResourceType = EResourceType::None;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	float ResourceAmount = 100.0f;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	float CollectionTime = 3.0f;

	UPROPERTY(EditAnywhere, Category="Resource Properties")
	int CollectionValue = 10;

	UPROPERTY(EditAnywhere, Category="Progression")
	bool UseRandomMesh = false;
};
