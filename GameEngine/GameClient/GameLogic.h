#pragma once

/// <summary>
/// 클라이언트 쪽 모든 매니저를 관리하는 로직 매니저
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;
class MonsterBase;
class MonsterManager;

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
	///몬스터 관련 함수들
	void SetCreateMonsterMemorySize(int CreateCount);						//몬스터 미리 생성
	MonsterBase* GetLifeMonter();											//행동하고있지 않은 몬스터를 한개 가져온다
	void CreateMonster(float CreateMaxTime,GameObject* CreatePointObject);	//해당 시간이 넘어갈때마다 오브젝트위치와 같은위치에 몬스터를 생성
private:
	const float CreateMonsterMaxTime = 5.0f;	//몬스터 생성 시간 max
private:
	//오브젝트 생성 클래스
	ObjectFactory*	FactoryGM; 
	MonsterManager* MonsterGM;
private:



	std::vector<GameObject*>	PotalList;
	std::vector<MonsterBase*>	MonsterList;
};

