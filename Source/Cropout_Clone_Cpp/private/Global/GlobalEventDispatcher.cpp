#include "Global/GlobalEventDispatcher.h"

UGlobalEventDispatcher::~UGlobalEventDispatcher()
{
	removeAllEventListeners();
}

void UGlobalEventDispatcher::Dispatch( const EGlobalEventType eventType )
{
	auto listener = getEventListener( eventType );
	if( listener )
	{
		listener->Broadcast();
	}
}

template <typename UserClass>
FDelegateHandle UGlobalEventDispatcher::AddListener( const EGlobalEventType eventType, UserClass* classInstance,
													typename TMemFunPtrType<false, UserClass, void()>::Type InFunc )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		return eventListener->AddRaw( classInstance, InFunc );
	}

	return FDelegateHandle();
}

template <typename UserClass>
FDelegateHandle UGlobalEventDispatcher::AddListenerUObject( const EGlobalEventType eventType, UserClass* classInstance,
															typename TMemFunPtrType<false, UserClass, void()>::Type InFunc )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		return eventListener->AddUObject( classInstance, InFunc );
	}

	return FDelegateHandle();
}

void UGlobalEventDispatcher::RemoveListener( const EGlobalEventType eventType, FDelegateHandle delegateHandle )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		eventListener->Remove( delegateHandle );
	}
}

template <typename UserClass>
void UGlobalEventDispatcher::RemoveAllListener( const EGlobalEventType eventType, UserClass* classInstance )
{
	auto eventListener = getEventListener( eventType );
	if( eventListener )
	{
		eventListener->RemoveAll( classInstance );
	}
}

TSharedPtr<FVoidDelegate> UGlobalEventDispatcher::getEventListener( const EGlobalEventType eventType )
{
	auto it = listeners.Find( eventType );
	if( it == nullptr )
	{
		listeners.Add( eventType, MakeShareable( new FVoidDelegate() ) );
	}
	return listeners[eventType];
}

void UGlobalEventDispatcher::removeAllEventListeners()
{
	for( auto listener : listeners )
	{
		listener.Value->Clear();
	}
}