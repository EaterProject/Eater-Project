#include "ClientObjectManager.h"
#include "ObjectFactory.h"
#include "MainHeader.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "PlayerCamera.h"
#include "GameObject.h"
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
	//���� ���丮 �ޱ�
	mFactory = Factory;

	//�⺻ ���� ������Ʈ��
	PlayerObject = mFactory->CreatePlayer();
	mFactory->CreateHealingDrone();

	//mFactory->CreateMonster(0, 0, 1, MONSTER_TYPE::MONSTER_A);

	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	//FindGameObjectTags("Potal", &PotalList);

	//�÷��̾� �浹�� ������Ʈ�� �����´�
	//FindGameObjectTag("AttackCollider");

	//SetCreateMonsterMemorySize(5);
}

void ClientObjectManager::Release()
{
	mFactory = nullptr;
}

void ClientObjectManager::SetCreateMonsterMemorySize(int CreateCount)
{
	float X = 10;
	float Y = 0;
	float Z = 10;
	for (int i = 0; i < CreateCount; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_A));
	}

	//Y = 5;
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//������ ���͸� ����Ʈ�� ��´�
	//	MonsterA_List.push_back(mFactory->CreateMonster(X, Y, Z + i, MONSTER_TYPE::MONSTER_B));
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

MonsterBase* ClientObjectManager::GetLifeMonter()
{
	//���� ������Ʈ ����Ʈ �߿��� �ൿ���� �ʴ� ���͸� ������
	//int MonsterMaxCount = (int)MonsterA_List.size();
	//for (int i = 0; i < MonsterMaxCount; i++)
	//{
	//	if (MonsterList[i]->isLife == false)
	//	{
	//		return MonsterList[i];
	//	}
	//}
	return nullptr;
}
