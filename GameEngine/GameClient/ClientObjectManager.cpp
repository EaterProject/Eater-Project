#include "ClientObjectManager.h"

//엔진 컨퍼넌트
#include "Player.h"
#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"


//클라이언트 컨퍼넌트
#include "Transform.h"
#include "Camera.h"
#include "PlayerCamera.h"
#include "ClientComponent.h"


ClientObjectManager::ClientObjectManager()
{
	mFactory		= nullptr;
	PlayerObject	= nullptr;
	DroneList		= nullptr;
}

ClientObjectManager::~ClientObjectManager()
{
	mFactory		= nullptr;
	PlayerObject	= nullptr;
}

void ClientObjectManager::Initialize(ObjectFactory* Factory)
{
	//생성 펙토리 받기
	mFactory = Factory;
	mFactory;

	//기본 생성 오브젝트들
	//PlayerObject = mFactory->CreatePlayer();
	//mFactory->CreateHealingDrone();

	
	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	//FindGameObjectTags("Potal", &Potal_List);

	//플레이어 충돌용 오브젝트를 가져온다
	//FindGameObjectTag("AttackCollider");

	SetCreateMonsterMemorySize(5);
}

void ClientObjectManager::Release()
{
	mFactory = nullptr;
}

auto ClientObjectManager::GetObjectPool(float x, float y, float z, CLIENT_OBJECT_TYPE Type)
{
	ClientComponent* Com = nullptr;
	switch (Type)
	{
	case CLIENT_OBJECT_TYPE::MONATER_A:
		//Com = GetLifeObject(MonsterA_List);
		return Com;
	case CLIENT_OBJECT_TYPE::MONATER_B:
		//Com = GetLifeObject(MonsterB_List);
		break;
	case CLIENT_OBJECT_TYPE::ATTACk_DRONE:
		//Com = GetLifeObject(AttackDrone_List);
		break;
	case CLIENT_OBJECT_TYPE::POTAL:
		//Com = GetLifeObject(Potal_List);
		break;
	case CLIENT_OBJECT_TYPE::BULLET:
		//Com = GetLifeObject(Bullet_List);
		break;
	}

	return Com;
}

void ClientObjectManager::SetObjectPool(ClientComponent* Com, CLIENT_OBJECT_TYPE Type)
{
	Com->isLife = false;
	Com->ReSet();
}

void ClientObjectManager::SetCreateMonsterMemorySize(int CreateCount)
{
	///몬스터 A 미리 할당
	for (int i = 0; i < 1; i++)
	{
		//생성한 몬스터를 리스트에 담는다
		//MonsterA_List.push_back(mFactory->CreateMonsterA(0, 5, 0));
	}

	///몬스터 B 미리 할당
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//생성한 몬스터를 리스트에 담는다
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
	//}

	///공격드론 미리 할당
	//AttackDrone_List.push_back(mFactory->CreateAttackDrone(3, 1, 0));
	

	///Bullet 미리 할당
	//for (int i = 0; i < 10; i++)
	//{
	//	Bullet_List.push_back(mFactory->CreateBullet(0, 4, 0));
	//}
}

void ClientObjectManager::CreateMonster(float CreateMaxTime, GameObject* CreatePointObject)
{
	//해당시간에 해당하는 오브젝트 위치로 몬스터를 생성시킴
	//static float CreateTime = 0;
	//CreateTime += GetDeltaTime();
	//
	//if (CreateTime >= CreateMaxTime)
	//{
	//	MonsterBase* Monster = GetLifeMonter();
	//	if (Monster == nullptr)
	//	{
	//		CreateTime -= CreateMaxTime;
	//		return;
	//	}
	//
	//	//몬스터를 해당 포탈위치로 이동시킴
	//	Transform* CreatePoint = CreatePointObject->GetTransform();
	//	Transform* MonsterPoint = Monster->gameobject->GetTransform();
	//	MonsterPoint->Position = CreatePoint->Position;
	//	CreateTime -= CreateMaxTime;
	//	Monster->isLife = true;
	//}
}


