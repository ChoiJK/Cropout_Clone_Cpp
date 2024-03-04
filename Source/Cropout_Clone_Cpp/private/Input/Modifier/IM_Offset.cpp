#include "Input/Modifier/IM_Offset.h"
#include "cmath"


FInputActionValue UIM_Offset::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                       FInputActionValue CurrentValue, float DeltaTime)
{
	FVector AsAxis3D = CurrentValue.Get<FInputActionValue::Axis3D>();
	const float offsetValue = AsAxis3D.X + Offset;
	Super::ModifyRaw_Implementation(nullptr,
	                                FInputActionValue(CurrentValue.GetValueType(),
	                                                  FVector(offsetValue, offsetValue, offsetValue)),
	                                0.0f);

	return CurrentValue;
}
