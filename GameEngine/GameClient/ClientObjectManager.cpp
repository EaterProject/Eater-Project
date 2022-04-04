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
#include "MonsterBase.h"


ClientObjectManager::ClientObjectManager()
{
	mFactory		= nullptr;
	PlayerObject	= nullptr;
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
	mFactory->CreateHealingDrone();

	
	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	FindGameObjectTags("Potal", &Potal_List);

	//플레이어 충돌용 오브젝트를 가져온다
	//FindGameObjectTag("AttackCollider");

	SetCreateMonsterMemorySize(5);
}

void ClientObjectManager::Release()
{
	mFactory = nullptr;
}

void ClientObjectManager::Update()
{
	CreateMonster(5, Potal_List[0]);



}

void ClientObjectManager::SetCreateMonsterMemorySize(int CreateCount)
{
	float X = 5;
	float Y = 1;
	float Z = 5;

	for (int i = 0; i < CreateCount; i++)
	{
		//생성한 몬스터를 리스트에 담는다
		MonsterA_List.push_back(mFactory->CreateMonster(X+i+2, Y, Z + i+2, MONSTER_TYPE::MONSTER_A));
	}

	//Y = 5;
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//생성한 몬스터를 리스트에 담는다
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
	//}

	//mFactory->CreateMonster(0, 3, 0, MONSTER_TYPE::MONSTER_A);
	for (int i = 0; i < 1; i++)
	{
		AttackDrone_List.push_back(mFactory->CreateAttackDrone(i, 1, 0));
	}
}

void ClientObjectManager::CreateMonster(float CreateMaxTime, GameObject* CreatePointObject)
{
	//해당시간에 해당하는 오브젝트 위치로 몬스터를 생성시킴
	static float CreateTime = 0;
	CreateTime += GetDeltaTime();
	
	if (CreateTime >= CreateMaxTime)
	{
		MonsterBase* Monster = GetLifeMonter();
		if (Monster == nullptr)
		{
			CreateTime -= CreateMaxTime;
			return;
		}
	
		//몬스터를 해당 포탈위치로 이동시킴
		Transform* CreatePoint = CreatePointObject->GetTransform();
		Transform* MonsterPoint = Monster->gameobject->GetTransform();
		MonsterPoint->Position = CreatePoint->Position;
		CreateTime -= CreateMaxTime;
		Monster->isLife = true;
	}
}

MonsterBase* ClientObjectManager::GetLifeMonter()
{
	//몬스터 오브젝트 리스트 중에서 행동하지 않는 몬스터를 가져옴
	int MonsterMaxCount = (int)MonsterA_List.size();
	for (int i = 0; i < MonsterMaxCount; i++)
	{
		if (MonsterA_List[i]->isLife == false)
		{
			return MonsterA_List[i];
		}
	}
	return nullptr;
}
