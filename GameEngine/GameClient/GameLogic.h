#pragma once

/// <summary>
/// 클라이언트 쪽 모든 매니저를 관리하는 로직 매니저
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

