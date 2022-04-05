#include "ClientObjectManager.h"

//���� ���۳�Ʈ
#include "Player.h"
#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"


//Ŭ���̾�Ʈ ���۳�Ʈ
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
	//���� ���丮 �ޱ�
	mFactory = Factory;
	mFactory;

	//�⺻ ���� ������Ʈ��
	//PlayerObject = mFactory->CreatePlayer();
	//mFactory->CreateHealingDrone();

	
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
	///���� A �̸� �Ҵ�
	for (int i = 0; i < 1; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		//MonsterA_List.push_back(mFactory->CreateMonsterA(0, 5, 0));
	}

	///���� B �̸� �Ҵ�
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//������ ���͸� ����Ʈ�� ��´�
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
	//}

	///���ݵ�� �̸� �Ҵ�
	//AttackDrone_List.push_back(mFactory->CreateAttackDrone(3, 1, 0));
	

	///Bullet �̸� �Ҵ�
	//for (int i = 0; i < 10; i++)
	//{
	//	Bullet_List.push_back(mFactory->CreateBullet(0, 4, 0));
	//}
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


