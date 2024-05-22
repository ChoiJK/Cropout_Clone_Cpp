#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"


static class StringUtils
{
public:
	static FString GenerateRandomStringWithSeed(int32 Length);
};
