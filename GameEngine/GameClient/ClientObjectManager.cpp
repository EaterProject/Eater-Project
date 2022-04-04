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
	//���� ���丮 �ޱ�
	mFactory = Factory;

	//�⺻ ���� ������Ʈ��
	PlayerObject = mFactory->CreatePlayer();
	mFactory->CreateHealingDrone();

	
	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("Potal", &Potal_List);

	//�÷��̾� �浹�� ������Ʈ�� �����´�
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
		//������ ���͸� ����Ʈ�� ��´�
		MonsterA_List.push_back(mFactory->CreateMonster(X+i+2, Y, Z + i+2, MONSTER_TYPE::MONSTER_A));
	}

	//Y = 5;
	//for (int i = 0; i < CreateCount; i++)
	//{
	//	//������ ���͸� ����Ʈ�� ��´�
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
	//�ش�ð��� �ش��ϴ� ������Ʈ ��ġ�� ���͸� ������Ŵ
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
	
		//���͸� �ش� ��Ż��ġ�� �̵���Ŵ
		Transform* CreatePoint = CreatePointObject->GetTransform();
		Transform* MonsterPoint = Monster->gameobject->GetTransform();
		MonsterPoint->Position = CreatePoint->Position;
		CreateTime -= CreateMaxTime;
		Monster->isLife = true;
	}
}

MonsterBase* ClientObjectManager::GetLifeMonter()
{
	//���� ������Ʈ ����Ʈ �߿��� �ൿ���� �ʴ� ���͸� ������
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
