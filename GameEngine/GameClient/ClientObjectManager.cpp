#include "ClientObjectManager.h"

//���� ���۳�Ʈ
#include "Player.h"
#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

//Ŭ���̾�Ʈ ���۳�Ʈ
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
	//���� ���丮 �ޱ�
	mFactory = Factory;

	//�⺻ ���� ������Ʈ��
	PlayerObject = mFactory->CreatePlayer();
	mFactory->CreateHealingDrone(0,1.5f,0);

	
	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	//FindGameObjectTags("Potal", &Potal_List);

	//�÷��̾� �浹�� ������Ʈ�� �����´�
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

	//������ Null
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
	///���� A �̸� �Ҵ�
	for (int i = 0; i < 1; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterA_List.push_back(mFactory->CreateMonsterA(0, 5, -10));
	}

	///���� B �̸� �Ҵ�
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//������ ���͸� ����Ʈ�� ��´�
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
	//}

	///���ݵ�� �̸� �Ҵ�
	AttackDrone_List.push_back(mFactory->CreateAttackDrone(2, 1, 0));
	AttackDrone_List.push_back(mFactory->CreateAttackDrone(2, 1, -8));
	

	///Bullet �̸� �Ҵ�
	for (int i = 0; i < 10; i++)
	{
		Bullet_List.push_back(mFactory->CreateBullet(0, 4, 0));
	}
}

void ClientObjectManager::CreateMonster(float CreateMaxTime, GameObject* CreatePointObject)
{
	//�ش�ð��� �ش��ϴ� ������Ʈ ��ġ�� ���͸� ������Ŵ
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
	//	//���͸� �ش� ��Ż��ġ�� �̵���Ŵ
	//	Transform* CreatePoint = CreatePointObject->GetTransform();
	//	Transform* MonsterPoint = Monster->gameobject->GetTransform();
	//	MonsterPoint->Position = CreatePoint->Position;
	//	CreateTime -= CreateMaxTime;
	//	Monster->isLife = true;
	//}
}


