#pragma once

#include "InputModifiers.h"
#include "IM_Normalize.generated.h"

UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "IM_Normalize"))
class UIM_Normalize : public UInputModifier
{
	GENERATED_BODY()

public:
	// Threshold below which input is ignored
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Settings, Config)
	float NormalizeRange = 90.f;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	                                                   FInputActionValue CurrentValue, float DeltaTime) override;
};
