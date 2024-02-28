#pragma once
#include "CoreMinimal.h"

class CROPOUT_CLONE_CPP_API FGlobalUtilFunctions
{
private:
	FGlobalUtilFunctions(); // Static Class로 사용할것

public:
	static FVector SteppedPosition(const FVector& position);
};
