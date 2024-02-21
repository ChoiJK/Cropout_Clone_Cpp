#pragma once
#include "CoreMinimal.h"

#include "GlobalEventDispatcher.generated.h"


DECLARE_MULTICAST_DELEGATE( FVoidDelegate );

enum class EGlobalEventType
{
	IslandGenComplete
};

UCLASS()
class CROPOUT_CLONE_CPP_API UGlobalEventDispatcher : public UObject
{
	GENERATED_BODY()

private:
	TMap<EGlobalEventType, TSharedPtr<FVoidDelegate>> listeners;

	TSharedPtr<FVoidDelegate> getEventListener( const EGlobalEventType eventType );
	void removeAllEventListeners();

public:
	~UGlobalEventDispatcher();

	template <typename UserClass>
	FDelegateHandle AddListener( const EGlobalEventType eventType, UserClass* classInstance,
								 typename TMemFunPtrType<false, UserClass, void()>::Type InFunc );

	template <typename UserClass>
	FDelegateHandle AddListenerUObject( const EGlobalEventType eventType, UserClass* classInstance,
										typename TMemFunPtrType<false, UserClass, void()>::Type InFunc );

	void RemoveListener( const EGlobalEventType eventType, FDelegateHandle delegateHandle );

	template <typename UserClass>
	void RemoveAllListener( const EGlobalEventType eventType, UserClass* classInstance );

	void Dispatch( const EGlobalEventType eventType );
};
