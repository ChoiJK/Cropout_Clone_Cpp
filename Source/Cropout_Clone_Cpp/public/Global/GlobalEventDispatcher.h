#pragma once
#include "CoreMinimal.h"


enum class EGlobalEventType
{
	IslandGenComplete,
	ChangedInputType
};

DECLARE_MULTICAST_DELEGATE(FVoidDelegate);

class CROPOUT_CLONE_CPP_API FGlobalEventDispatcher
{
private:
	TMap<EGlobalEventType, TSharedPtr<FVoidDelegate>> listeners;

	TSharedPtr<FVoidDelegate> getEventListener(const EGlobalEventType eventType);
	void removeAllEventListeners();

public:
	~FGlobalEventDispatcher();

	template <typename UserClass>
	FDelegateHandle AddListener(const EGlobalEventType eventType, UserClass* classInstance,
	                            typename TMemFunPtrType<false, UserClass, void()>::Type InFunc)
	{
		auto eventListener = getEventListener(eventType);
		if(eventListener)
		{
			return eventListener->AddRaw(classInstance, InFunc);
		}

		return FDelegateHandle();
	}

	template <typename UserClass>
	FDelegateHandle AddListenerUObject(const EGlobalEventType eventType, UserClass* classInstance,
	                                   typename TMemFunPtrType<false, UserClass, void()>::Type InFunc)
	{
		auto eventListener = getEventListener(eventType);
		if(eventListener)
		{
			return eventListener->AddUObject(classInstance, InFunc);
		}

		return FDelegateHandle();
	}

	void RemoveListener(const EGlobalEventType eventType, FDelegateHandle delegateHandle);

	template <typename UserClass>
	void RemoveAllListener(const EGlobalEventType eventType, UserClass* classInstance);

	void Dispatch(const EGlobalEventType eventType);
};
