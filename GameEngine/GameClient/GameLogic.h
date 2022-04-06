#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;
class MonsterBase;
class ClientObjectManager;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

public:
	void Initialize();
	void Release();
	void Update();
private:
	ObjectFactory*			FactoryGM;	//������Ʈ ���� Ŭ����
	ClientObjectManager*	ObjectGM;	//������Ʈ ���� Ŭ����
private:
	std::vector<GameObject*>	PotalList;
	std::vector<MonsterBase*>	MonsterList;
};

