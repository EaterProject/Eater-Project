#pragma once
#include <string>

/// <summary>
/// ������ �����ʿ��� �����Ҷ� �̸��� �Լ������͵鸸 �Ѱ��ְ�
///  ������������ ��ũ���Ŵ������� ���ش�
/// </summary>

#include "EaterEngineDLL.h"
class Scene
{
public:
	EATER_ENGINEDLL Scene();
	EATER_ENGINEDLL virtual ~Scene();

	//���� �����ɋ� ȣ��
	virtual void Awake()=0;
	virtual void Update()=0;
	virtual void End()=0;

	EATER_ENGINEDLL virtual void ThreadFunction();
	EATER_ENGINEDLL virtual void Run();

	std::string Name;
	bool ThreadRun;
private:
};


