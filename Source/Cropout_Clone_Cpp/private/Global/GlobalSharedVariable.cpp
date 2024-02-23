#include "Global/GlobalSharedVariable.h"
#include "Core/CropoutGameInstance.h"

GlobalSharedVariable::GlobalSharedVariable(UCropoutGameInstance* gameInstance)
{
	this->GameInstance = gameInstance;
	this->EventDispatcher = GameInstance->GetGlobalEventDispatcher();
}

void GlobalSharedVariable::SetScaleUpDelay(float delay)
{
	// @ todo : RX로 작업하고싶으나 본 프로젝트에서는 너무 무거워보임
	// 일단은 혼자하니까 이렇게 처리. 게임 프로젝트로 전환시 RX로 변경하는게 1순위일듯.
	ScaleUpDelay = delay;
	if(const TSharedPtr<GlobalEventDispatcher> dispatcher = EventDispatcher.Pin())
	{
		dispatcher->Dispatch(EGlobalEventType::ScaleUp);
	}
}
