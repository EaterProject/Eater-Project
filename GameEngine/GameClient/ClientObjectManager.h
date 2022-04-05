#pragma once
#include <vector>

class ClientComponent;
class ObjectFactory;
class GameObject;
class Transform;

class HealingDrone;
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
	MonsterA* GetMonsterA();
	MonsterB* GetMonsterB();
private:
	void SetCreateMonsterMemorySize(int CreateCount);	//필요한 오브젝트들 미리 생성
	void CreateMonster(float CreateMaxTime, GameObject* CreatePointObject);
private:
	GameObject* PlayerObject;
private:
	std::vector<MonsterA*>			MonsterA_List;
	std::vector<MonsterB*>			MonsterB_List;
	std::vector<AttackDrone*>		AttackDrone_List;
	std::vector<Potal*>				Potal_List;
	std::vector<Bullet*>			Bullet_List;
	HealingDrone* DroneList;
private:
	ObjectFactory* mFactory;
};



