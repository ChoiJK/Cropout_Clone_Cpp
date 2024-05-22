#include "Util/StringUtils.h"

FString StringUtils::GenerateRandomStringWithSeed(int32 Length)
{
	FString RandomString;
	const FString Alphabet = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

	for(int32 i = 0; i < Length; ++i)
	{
		int32 Index = FMath::RandRange(0, Alphabet.Len() - 1);
		RandomString.AppendChar(Alphabet[Index]);
	}

	return RandomString;
}
