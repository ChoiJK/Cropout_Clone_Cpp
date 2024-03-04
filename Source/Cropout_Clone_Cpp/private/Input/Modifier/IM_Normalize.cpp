#include "Input/Modifier/IM_Normalize.h"
#include "cmath"


FInputActionValue UIM_Normalize::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                          FInputActionValue CurrentValue, float DeltaTime)
{
	FVector AsAxis3D = CurrentValue.Get<FInputActionValue::Axis3D>();
	const float normalizedValue = FMath::Clamp(AsAxis3D.X / NormalizeRange, -1.f, 1.f);

	Super::ModifyRaw_Implementation(PlayerInput,
	                                FInputActionValue(CurrentValue.GetValueType(),
	                                                  FVector(normalizedValue, normalizedValue, normalizedValue)),
	                                DeltaTime);

	return CurrentValue;
}
