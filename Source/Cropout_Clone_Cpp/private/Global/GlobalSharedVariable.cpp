#include "Global/GlobalSharedVariable.h"
#include "Core/CropoutGameInstance.h"

FGlobalSharedVariable::FGlobalSharedVariable(UCropoutGameInstance* gameInstance)
{
	this->GameInstance = gameInstance;
	this->EventDispatcher = GameInstance->GetGlobalEventDispatcher();
}
