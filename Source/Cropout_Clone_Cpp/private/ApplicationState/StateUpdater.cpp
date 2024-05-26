#include "ApplicationState/StateUpdater.h"

void UStateUpdater::SetProcessUpdate(const StateProcessFunction& processFunction)
{
	ProcessUpdate = processFunction;
}

void UStateUpdater::DoUpdate() const
{
	if(ProcessUpdate)
	{
		ProcessUpdate();
	}
}
