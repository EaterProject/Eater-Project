#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
/// </summary>
#include <vector>

class ObjectFactory;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();
	void Initialize();
	void Release();
private:
	ObjectFactory* Factory;

};

