#pragma once

#include <functional>
#include "EngineData.h"
#include <vector>

/// <summary>
///	��ȯ�������� �Լ��������� ������ �������ֱ����� map ���� vector �κ���
/// </summary>
class Component;
class Delegate_Map
{
public:
	Delegate_Map();
	~Delegate_Map();
	
	void Push(ComponentFunctionData temp);	//���۳�Ʈ �Լ�����Ʈ �߰�
	void Pop(Component* Key);				//���۳�Ʈ �Լ������� ����
	void Clear();							//��λ���
	void Play();							////���۳�Ʈ �Լ������� �� ������ ����
	void PlayOnce();						////���۳�Ʈ �Լ������� ȣ��� �ѹ�!!! ����
private:
	std::vector<ComponentFunctionData> FunctionList00;
	std::vector<ComponentFunctionData> FunctionList01;
	std::vector<ComponentFunctionData> FunctionList02;
	std::vector<ComponentFunctionData> FunctionList03;
	std::vector<ComponentFunctionData> FunctionList04;

	const int MaxCount = 5;
};

