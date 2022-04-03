#pragma once
#include <vector>

class MonsterBase;
class ObjectFactory;
class GameObject;

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
	std::vector<MonsterBase*> MonsterA_List;
	std::vector<MonsterBase*> MonsterB_List;
	


	ObjectFactory* mFactory;
};

