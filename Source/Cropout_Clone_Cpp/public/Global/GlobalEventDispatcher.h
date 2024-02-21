#pragma once
#include "CoreMinimal.h"


enum class EGlobalEventType
{
	IslandGenComplete
};

DECLARE_MULTICAST_DELEGATE( FVoidDelegate );

class CROPOUT_CLONE_CPP_API GlobalEventDispatcher
{

private:
	TMap<EGlobalEventType, TSharedPtr<FVoidDelegate>> listeners;

	TSharedPtr<FVoidDelegate> getEventListener( const EGlobalEventType eventType );
	void removeAllEventListeners();

public:
	~GlobalEventDispatcher();

	template <typename UserClass>
	inline FDelegateHandle AddListener( const EGlobalEventType eventType, UserClass* classInstance,
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
	inline FDelegateHandle AddListenerUObject( const EGlobalEventType eventType, UserClass* classInstance,
										typename TMemFunPtrType<false, UserClass, void()>::Type InFunc )
	{
		auto eventListener = getEventListener( eventType );
		if( eventListener )
		{
			return eventListener->AddUObject( classInstance, InFunc );
		}

		return FDelegateHandle();
	}

	void RemoveListener( const EGlobalEventType eventType, FDelegateHandle delegateHandle );

	template <typename UserClass>
	void RemoveAllListener( const EGlobalEventType eventType, UserClass* classInstance );

	void Dispatch( const EGlobalEventType eventType );
};
