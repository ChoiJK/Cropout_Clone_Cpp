#pragma once

#include <memory>

#include "SimpleRxReactiveProperty.h"
#include "SimpleRxCompositeDisposable.h"

template <typename T>
class SimpleRxSubscribeHandle : public std::enable_shared_from_this<SimpleRxSubscribeHandle<T>>
{
	int SubscribeID = -1;
	std::shared_ptr<SimpleRxReactiveProperty<T>> ReactiveProperty = nullptr;

public:
	SimpleRxSubscribeHandle(std::shared_ptr<SimpleRxReactiveProperty<T>> reactiveProperty, int subscribeID)
	{
		ReactiveProperty = reactiveProperty;
		SubscribeID = subscribeID;
	}

	void AddTo(const std::shared_ptr<SimpleRxCompositeDisposable>& CompositeDisposable)
	{
		CompositeDisposable->AddHandle<T>(this->shared_from_this());
	}

	void Dispose()
	{
		if(ReactiveProperty)
		{
			ReactiveProperty->Unsubscribe(SubscribeID);
		}
	}
};
