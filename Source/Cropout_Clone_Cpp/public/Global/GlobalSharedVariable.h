#pragma once

class FGlobalEventDispatcher;
class UCropoutGameInstance;

class CROPOUT_CLONE_CPP_API FGlobalSharedVariable
{
	FGlobalSharedVariable(UCropoutGameInstance* gameInstance);

private:
	UCropoutGameInstance* GameInstance;
	TWeakPtr<FGlobalEventDispatcher> EventDispatcher;

public:

private:
};
