#pragma once
#include <vector>

class MonsterBase;
class ObjectFactory;
class GameObject;
class HealingDrone;
class Transform;

class ClientObjectManager
{
public:
	ClientObjectManager();
	~ClientObjectManager();

	void Initialize(ObjectFactory* Factory);
	void Release();
private:
	void SetCreateMonsterMemorySize(int CreateCount);	//몬스터 미리 생성

	void CreateMonster(float CreateMaxTime, GameObject* CreatePointObject);
	MonsterBase* GetLifeMonter();

private:
	GameObject* PlayerObject;
private:
	std::vector<MonsterBase*> MonsterA_List;
	std::vector<MonsterBase*> MonsterB_List;
	HealingDrone* DroneList;
private:
	ObjectFactory* mFactory;
};

