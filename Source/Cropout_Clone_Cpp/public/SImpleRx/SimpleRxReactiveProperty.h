#pragma once

#include <vector>
#include <functional>
#include <map>

#include "SimpleReactiveManager.h"

template <typename T>
class SimpleRxSubscribeHandle;

template <typename T>
class SimpleRxReactiveProperty : public std::enable_shared_from_this<SimpleRxReactiveProperty<T>>
{
	T Value;
	int SubscribeID = 0;
	std::map<int, std::function<void(T)>> OnValueChanged;

	USimpleReactiveManager* SimpleReactiveManager;

	void Initialize()
	{
		SimpleReactiveManager = USimpleReactiveManager::GetInstance();
	}

	void Notification()
	{
		for(auto& onValueChanged : OnValueChanged)
		{
			onValueChanged.second(Value);
		}
	}

	int GetSubscribeID()
	{
		return SubscribeID++;
	}

public:
	SimpleRxReactiveProperty() : Value(T())
	{
		Initialize();
	}

	SimpleRxReactiveProperty(T value) : Value(value)
	{
		Initialize();
	}

	T GetValue() const
	{
		return Value;
	}

	void SetValue(T value)
	{
		if(Value != value)
		{
			Value = value;
			Notification();
		}
	}

	void SetValueSilently(T value)
	{
		Value = value;
	}

	void SetValueNotifyOnTick(T value)
	{
		// TODO: Implement this
		if(Value != value)
		{
			Value = value;
		}
	}

	std::shared_ptr<SimpleRxSubscribeHandle<T>> Subscribe(std::function<void(T)> onValueChanged)
	{
		int subscribeID = GetSubscribeID();
		std::shared_ptr<SimpleRxSubscribeHandle<T>> handle = std::make_shared<SimpleRxSubscribeHandle<T>>(
			this->shared_from_this(), subscribeID);
		OnValueChanged.insert({subscribeID, onValueChanged});
		return handle;
	}

	std::shared_ptr<SimpleRxSubscribeHandle<T>> SubscribeAndNotify(std::function<void(T)> onValueChanged)
	{
		std::shared_ptr<SimpleRxSubscribeHandle<T>> handle = Subscribe(onValueChanged);
		onValueChanged(Value);
		return handle;
	}


	void Unsubscribe(std::function<void(T)> func)
	{
		std::vector<int> keysToRemove;

		// 맵을 순회하면서 해당 값을 갖는 모든 키를 찾습니다.
		for(const auto& pair : OnValueChanged)
		{
			if(pair.second == func)
			{
				keysToRemove.push_back(pair.first);
			}
		}

		// 찾은 키들을 이용해 해당 항목들을 삭제합니다.
		for(const int& key : keysToRemove)
		{
			OnValueChanged.erase(key);
		}
	}

	void Unsubscribe(int subscribeID)
	{
		OnValueChanged.erase(subscribeID);
	}
};
