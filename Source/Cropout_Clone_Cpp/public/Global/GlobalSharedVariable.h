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
	void SetScaleUpDelay(float delay);
	float GetScaleUpDelay() const { return ScaleUpDelay; }

private:
	float ScaleUpDelay = 0.0f;
};
