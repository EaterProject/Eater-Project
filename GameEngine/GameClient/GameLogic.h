#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();
	void Initialize();
	void Release();
	void Update();
private:
	ObjectFactory* Factory;

	std::vector<GameObject*> PotalList;
};

