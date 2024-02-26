#include "../../public/Global/GlobalEventDispatcher.h"

FGlobalEventDispatcher::~FGlobalEventDispatcher()
{
	removeAllEventListeners();
}

void FGlobalEventDispatcher::Dispatch(const EGlobalEventType eventType)
{
	auto listener = getEventListener(eventType);
	if(listener)
	{
		listener->Broadcast();
	}
}

void FGlobalEventDispatcher::RemoveListener(const EGlobalEventType eventType, FDelegateHandle delegateHandle)
{
	auto eventListener = getEventListener(eventType);
	if(eventListener)
	{
		eventListener->Remove(delegateHandle);
	}
}

template <typename UserClass>
void FGlobalEventDispatcher::RemoveAllListener(const EGlobalEventType eventType, UserClass* classInstance)
{
	auto eventListener = getEventListener(eventType);
	if(eventListener)
	{
		eventListener->RemoveAll(classInstance);
	}
}

TSharedPtr<FVoidDelegate> FGlobalEventDispatcher::getEventListener(const EGlobalEventType eventType)
{
	auto it = listeners.Find(eventType);
	if(it == nullptr)
	{
		listeners.Add(eventType, MakeShareable(new FVoidDelegate()));
	}
	return listeners[eventType];
}

void FGlobalEventDispatcher::removeAllEventListeners()
{
	for(auto listener : listeners)
	{
		listener.Value->Clear();
	}
}
