#pragma once

#include "InputModifiers.h"
#include "IM_Offset.generated.h"

UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "IM_Offset"))
class UIM_Offset : public UInputModifier
{
	GENERATED_BODY()

public:
	// Threshold below which input is ignored
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Settings, Config)
	float Offset = 0.2f;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	                                                   FInputActionValue CurrentValue, float DeltaTime) override;
};
