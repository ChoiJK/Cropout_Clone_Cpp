#include "Core/ApplicationState/StateUpdater.h"

void UStateUpdater::SetProcessUpdate(const StateProcess& processFunction)
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
