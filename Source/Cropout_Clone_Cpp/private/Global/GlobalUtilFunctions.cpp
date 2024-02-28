#include "Global/GlobalUtilFunctions.h"

FVector FGlobalUtilFunctions::SteppedPosition(const FVector& position)
{
	FVector retVal = position / 200.f;
	retVal.X = FMath::RoundToFloat(retVal.X) * 200.f;
	retVal.Y = FMath::RoundToFloat(retVal.Y) * 200.f;
	retVal.Z = 0.f;

	return retVal;
}
