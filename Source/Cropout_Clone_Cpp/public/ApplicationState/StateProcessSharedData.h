#pragma once
#include "CoreMinimal.h"
#include "StateProcessSharedData.generated.h"

class UCommonActivatableWidget;

UCLASS()
class CROPOUT_CLONE_CPP_API UStateProcessSharedData : public UObject
{
	GENERATED_BODY()

	UCommonActivatableWidget* CurrentActivatableWidget;

public:
	void SetCurrentActivatableWidget(UCommonActivatableWidget* commonActivatableWidget);
	UCommonActivatableWidget* GetCurrentActivatableWidget();
};
