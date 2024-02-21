
#include "Global/GlobalEventDispatcher.h"

GlobalEventDispatcher::~GlobalEventDispatcher()
{
	removeAllEventListeners();
}

void GlobalEventDispatcher::Dispatch( const EGlobalEventType eventType )
{
	auto listener = getEventListener( eventType );
	if( listener )
	{
		listener->Broadcast();
	}
}

void GlobalEventDispatcher::RemoveListener( const EGlobalEventType eventType, FDelegateHandle delegateHandle )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		eventListener->Remove( delegateHandle );
	}
}

template <typename UserClass>
void GlobalEventDispatcher::RemoveAllListener( const EGlobalEventType eventType, UserClass* classInstance )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		eventListener->RemoveAll( classInstance );
	}
}

TSharedPtr<FVoidDelegate> GlobalEventDispatcher::getEventListener( const EGlobalEventType eventType )
{
	auto it = listeners.Find( eventType );
	if( it == nullptr )
	{
		listeners.Add( eventType, MakeShareable( new FVoidDelegate() ) );
	}
	return listeners[eventType];
}

void GlobalEventDispatcher::removeAllEventListeners()
{
	for( auto listener : listeners )
	{
		listener.Value->Clear();
	}
}