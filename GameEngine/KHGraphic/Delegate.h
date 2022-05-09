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

using namespace std;

template<typename T, typename ..._Args>
class FunctionEvent
{
public:
	FunctionEvent() = default;

	template<typename _Func, typename ..._Types>
	FunctionEvent(_Func&& _func, _Types&&... _args)
	{
		pfunction = std::bind_front(std::forward<_Func>(_func), std::forward<_Types>(_args)...);

		pfunctionEvent.push_back(pfunction);
	}

public:
	template<typename ..._Types>
	void operator()(_Types&&... _types)
	{
		for (auto& function : pfunctionEvent)
		{
			function(std::forward<_Types>(_types)...);
		}
	}

public:
	template<typename ..._Types>
	static void EventCall(_Types&&... _types)
	{
		for (auto& function : pfunctionEvent)
		{
			function(std::forward<_Types>(_types)...);
		}
	}

public:
	std::function<T(_Args...)> pfunction;

	static std::vector<std::function<T(_Args...)>> pfunctionEvent;
};

template<typename T, typename ..._Args>
std::vector<std::function<T(_Args...)>> FunctionEvent<T, _Args...>::pfunctionEvent;



template<typename T, typename ..._Args>
class FunctionEventList
{
public:
	std::vector<std::function<T(_Args...)>> pfunctionEvent;

public:
	template<typename _Func, typename ..._Types>
	void Push(_Func&& _func, _Types&&... _args)
	{
		pfunctionEvent.push_back(std::bind_front(std::forward<_Func>(_func), std::forward<_Types>(_args)...));
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
		pfunctionEvent.push_back(func.pfunction);
	}

	template<typename _Func, typename ..._Types>
	void operator-=(FunctionEvent<_Func, _Types...>& func)
	{

	}

	template<typename ..._Types>
	void operator()(_Types&&... _types)
	{
		for (auto& function : pfunctionEvent)
		{
			function(std::forward<_Types>(_types)...);
		}
	}
};





class A
{
public:
	int Add(int a, int c)
	{
		return 0;
	}
};

int Add(int a, int c)
{
	return a;
};

void Add2(int a, int b, int c)
{

}

int main(void)
{
	A a;

	FunctionEventList<int, int, int> eventFunction;
	FunctionEvent<int, int, int> OneFunction(&A::Add, &a);
	FunctionEvent<int, int, int> OneFunction1(&Add);

	FunctionEvent<void, int, int, int> OneFunction2(&Add2);

	eventFunction += OneFunction;
	eventFunction += OneFunction1;

	/// Function Version
	//eventFunction.Push(Add);
	//eventFunction.Push(&A::Add, &a);
	//
	//eventFunction.EventCall(10, 10);

	/// Operator Version
	FunctionEvent<int, int, int>::EventCall(10, 2);
	eventFunction(1, 20);

	return 0;
}

class Delegate
{

};



