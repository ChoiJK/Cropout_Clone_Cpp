#pragma once

class GlobalEventDispatcher;
class UCropoutGameInstance;

class CROPOUT_CLONE_CPP_API GlobalSharedVariable
{
	GlobalSharedVariable(UCropoutGameInstance* gameInstance);

private:
	UCropoutGameInstance* GameInstance;
	TWeakPtr<GlobalEventDispatcher> EventDispatcher;

public:
	void SetScaleUpDelay(float delay);
	float GetScaleUpDelay() const { return ScaleUpDelay; }

private:
	float ScaleUpDelay = 0.0f;
};
