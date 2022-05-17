#pragma once
#include <iostream>
#include <utility>
#include <functional>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>
#include <functional>


template<typename T, typename ..._Args>
class FunctionEvent
{
public:
	FunctionEvent() = default;

	template<typename _Func, typename ..._Types>
	FunctionEvent(_Func&& _func, _Types&&... _args)
	{
		pfunction = std::bind_front(std::forward<_Func>(_func), std::forward<_Types>(_args)...);
	}

public:
	template<typename _Func, typename ..._Types>
	void Bind(_Func&& _func, _Types&&... _args)
	{
		std::function<T(_Args...)> result = std::bind_front(std::forward<_Func>(_func), std::forward<_Types>(_args)...);

		pfunction = result;
	}

public:
	bool operator==(const FunctionEvent<T, _Args...>& func)
	{
		if (this->pfunction.target_type() == func.pfunction.target_type())
			return true;

		return false;
	}

	void operator()(_Args... _types)
	{
		pfunction(std::forward<_Args>(_types)...);
	}

public:
	std::function<T(_Args...)> pfunction;
};



template<typename T, typename ..._Args>
class FunctionEventList
{
public:
	template<typename _Func, typename ..._Types>
	FunctionEvent<_Func, _Types...> Push(_Func&& _func, _Types&&... _args)
	{
		auto pFunction = std::bind_front(std::forward<_Func>(_func), std::forward<_Types>(_args)...);

		pfunctionEvent.push_back(pFunction);

		return pFunction;
	}

	template<typename ..._Types>
	void EventCall(_Types&&... _types)
	{
		for (auto& function : pfunctionEvent)
		{
			function(std::forward<_Types>(_types)...);
		}
	}

public:
	template<typename _Func, typename ..._Types>
	void operator+=(FunctionEvent<_Func, _Types...>& func)
	{
		pfunctionEvent.push_back(func);
	}

	template<typename _Func, typename ..._Types>
	void operator-=(FunctionEvent<_Func, _Types...>& func)
	{
		int index = 0;
		for (auto& function : pfunctionEvent)
		{
			if (function == func)
			{
				pfunctionEvent.erase(std::next(pfunctionEvent.begin(), index));
				break;
			}
			index++;
		}
	}

	void operator()(_Args... _types)
	{
		for (auto& function : pfunctionEvent)
		{
			function(std::forward<_Args>(_types)...);
		}
	}

private:
	std::vector<FunctionEvent<T, _Args...>> pfunctionEvent;
};
