#pragma once

#include <any>
#include <vector>
#include <functional>
#include "SimpleRx/SimpleRxSubscribeHandle.h"

class SimpleRxCompositeDisposable
{
	std::vector<std::any> handles;
	std::vector<std::function<void()>> disposables;

public:
	~SimpleRxCompositeDisposable()
	{
		Dispose();
	}

	template <typename T>
	void AddHandle(std::shared_ptr<SimpleRxSubscribeHandle<T>> handle)
	{
		// any는 shared_ptr의 ref_count를 유지함
		handles.push_back(handle);

		disposables.push_back([=]()
		{
			handle->Dispose();
		});
	}

	void Dispose()
	{
		for(auto& disposable : disposables)
		{
			disposable();
		}
		handles.clear();
		disposables.clear();
	}
};
