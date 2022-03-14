#pragma once

#include <functional>
#include "EngineData.h"
#include <vector>

/// <summary>
///	순환과정에서 함수포인터의 순서를 보장해주기위해 map 에서 vector 로변경
/// </summary>
class Component;
class Delegate_Map
{
public:
	Delegate_Map();
	~Delegate_Map();
	
	void Push(ComponentFunctionData temp);	//컨퍼넌트 함수포인트 추가
	void Pop(Component* Key);				//컨퍼넌트 함수포인터 삭제
	void Clear();							//모두삭제
	void Play();							////컨퍼넌트 함수포인터 매 프레임 실행
	void PlayOnce();						////컨퍼넌트 함수포인터 호출시 한번!!! 실행
private:
	std::vector<ComponentFunctionData> FunctionList00;
	std::vector<ComponentFunctionData> FunctionList01;
	std::vector<ComponentFunctionData> FunctionList02;
	std::vector<ComponentFunctionData> FunctionList03;
	std::vector<ComponentFunctionData> FunctionList04;

	const int MaxCount = 5;
};

