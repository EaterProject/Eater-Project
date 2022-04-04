#pragma once

/// <summary>
/// 오브젝트들 생성을 관리하는 오브젝트 펙토리
/// </summary>
#include <vector>
enum class MONSTER_TYPE
{
	MONSTER_A, //물기공격 몬스터 
	MONSTER_B, //자폭공격 몬스터
};

class GameObject;
class MonsterBase;
class AttackDrone;
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	void Initialize();
	void Release();


	GameObject* CreateCameraPlayer();												//플레이어 카메라 객체를 생성
	GameObject* CreatePlayer();														//플레이어 객체를 생성

	MonsterBase* CreateMonster(float x, float y, float z , MONSTER_TYPE Type);		//몬스터 객체를 생성
	GameObject* CreateMana();														//마나석 객체를 생성
	GameObject* CreateHealingDrone();												//힐러 드론 객체를 생성
	AttackDrone* CreateAttackDrone(float x, float y, float z);						//공격 드론 객체를 생성
	GameObject* CreatePortal();														//몬스터객체가 나오는 포탈 생성
private:
	//게임상에 한개만 존재하는 오브젝트를 따로 보관
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
};

