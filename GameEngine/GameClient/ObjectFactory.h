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
class MonsterA;
class MonsterB;
class AttackDrone;
class HealingDrone;
class Bullet;
class ManaStone;
class ClientObjectManager;
class Potal;


class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Initialize(ClientObjectManager* mGM);
	void Release();

private:
	GameObject*				CreatePlayer();									//플레이어 객체를 생성
	Bullet*					CreateBullet(float x,float y,float z);			//총알 객체를 생성
	MonsterA*				CreateMonsterA(float x, float y, float z);		//몬스터 객체를 생성
	MonsterB*				CreateMonsterB(float x, float y, float z);		//몬스터 객체를 생성
	ManaStone*				CreateManaStone();								//마나석 객체를 생성
	HealingDrone*			CreateHealingDrone();							//힐러 드론 객체를 생성
	AttackDrone*			CreateAttackDrone(float x, float y, float z);	//공격 드론 객체를 생성
	Potal*					CreatePortal();									//몬스터객체가 나오는 포탈 생성
private:
	//게임상에 한개만 존재하는 오브젝트를 따로 보관
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;

	ClientObjectManager* mOBJ_GM;
};

