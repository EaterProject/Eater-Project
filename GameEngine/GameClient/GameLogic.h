#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;
class MonsterBase;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	

	void Initialize();
	void Release();
	void Update();
private:
	void SetCreateMonsterMemorySize(int CreateCount);
	void LifeMonsterCheck();
	
private:
	float CreateMonsterTime;
	const float CreateMonsterMaxTime = 5.0f;

private:
	ObjectFactory* Factory;

	std::vector<GameObject*> PotalList;
	std::vector<MonsterBase*> MonsterList;
};

