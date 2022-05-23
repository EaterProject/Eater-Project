#pragma once
#include <vector>

class ClientComponent;
class ObjectFactory;
class GameObject;
class Transform;

class Drone;
class AttackDrone;
class Bullet;
class MonsterA;
class MonsterB;
class Potal;

enum class  CLIENT_OBJECT_TYPE
{
	MONATER_A,
	MONATER_B,
	ATTACk_DRONE,
	POTAL,
	BULLET,
};

class ClientObjectManager
{
public:
	ClientObjectManager();
	~ClientObjectManager();

	void Initialize(ObjectFactory* Factory);
	void Release();

	Bullet* GetBullet();
	//MonsterA* GetMonsterA();
	//MonsterB* GetMonsterB();
	Transform* GetPlayerTransform();
private:
	void CreateObjectMemorySize();	//필요한 오브젝트들 미리 생성
	void OnActivePotal(bool isActive,int index = -1);
private:
	GameObject* PlayerObject = nullptr;
private:
	std::vector<MonsterA*>			MonsterA_List;
	std::vector<MonsterB*>			MonsterB_List;
	std::vector<AttackDrone*>		AttackDrone_List;
	std::vector<Bullet*>			Bullet_List;
	std::vector<Potal*>				Potal_List;
	std::vector<GameObject*>		ManaPoint_List;
	Drone* DroneList;
private:
	const int CreateMonsterACount		= 0;		//몬스터A 생성할 카운터
	const int CreateMonsterBCount		= 0;		//몬스터B 생성할 카운터
	const int CreateAttackDroneCount	= 0;		//공격드론 생성할 카운터
	const int CreateBulletCount			= 0;		//총알 생성할 카운터
private:
	ObjectFactory* mFactory;
};



