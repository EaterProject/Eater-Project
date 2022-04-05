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

	auto GetObjectPool(float x,float y, float z, CLIENT_OBJECT_TYPE Type);
	void SetObjectPool(ClientComponent* Com, CLIENT_OBJECT_TYPE Type);
private:
	void SetCreateMonsterMemorySize(int CreateCount);	//필요한 오브젝트들 미리 생성
	void CreateMonster(float CreateMaxTime, GameObject* CreatePointObject);

	template <typename T>
	T* GetLifeObject(std::vector<T>& List);
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

template<typename T>
inline T* ClientObjectManager::GetLifeObject(std::vector<T>& List)
{
	int Size = (int)List.size();
	for (int i = 0; i < Size; i++)
	{
		if (List[i].isLife == true)
		{
			return List[i];
		}
	}

	return nullptr;
}
