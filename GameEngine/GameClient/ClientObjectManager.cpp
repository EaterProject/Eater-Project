#include "ClientObjectManager.h"

//엔진 컨퍼넌트
#include "Player.h"
#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

//클라이언트 컨퍼넌트
#include "PlayerCamera.h"
#include "ClientComponent.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "Potal.h"
#include "HealingDrone.h"
#include "AttackDrone.h"
#include "Bullet.h"


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

	//기본 생성 오브젝트들
	PlayerObject = mFactory->CreatePlayer();
	mFactory->CreateHealingDrone(0,1.5f,0);

	
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

Bullet* ClientObjectManager::GetBullet()
{
	int Size = (int)Bullet_List.size();
	for (int i = 0; i < Size; i++)
	{
		if (Bullet_List[i]->GetLife() == false)
		{
			return Bullet_List[i];
		}
	}

	//없으면 Null
	return nullptr;
}

MonsterA* ClientObjectManager::GetMonsterA()
{
	return nullptr;
}

MonsterB* ClientObjectManager::GetMonsterB()
{
	return nullptr;
}



void ClientObjectManager::SetCreateMonsterMemorySize(int CreateCount)
{
	///몬스터 A 미리 할당
	for (int i = 0; i < 1; i++)
	{
		//생성한 몬스터를 리스트에 담는다
		MonsterA_List.push_back(mFactory->CreateMonsterA(0, 5, -10));
	}

	///몬스터 B 미리 할당
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//생성한 몬스터를 리스트에 담는다
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
	//}

	///공격드론 미리 할당
	AttackDrone_List.push_back(mFactory->CreateAttackDrone(2, 1, 0));
	AttackDrone_List.push_back(mFactory->CreateAttackDrone(2, 1, -8));
	

	///Bullet 미리 할당
	for (int i = 0; i < 10; i++)
	{
		Bullet_List.push_back(mFactory->CreateBullet(0, 4, 0));
	}
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


