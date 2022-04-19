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
	mFactory->CreateHealingDrone(0, 1.5f, 0);

	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("Potal", &PotalPoint_List);

	CreateObjectMemorySize();
	OnActivePotal(true);

	GameObject* obj = InstanceTerrain();
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
			Bullet_List[i]->SetLife(true);
			return Bullet_List[i];
		}
	}

	//������ Null
	return nullptr;
}

MonsterA* ClientObjectManager::GetMonsterA()
{
	int Size = (int)MonsterA_List.size();
	for (int i = 0; i < Size; i++)
	{
		if (MonsterA_List[i]->GetLife() == false)
		{
			MonsterA_List[i]->SetLife(true);
			MonsterA_List[i]->ReSet();
			return MonsterA_List[i];
		}
	}

	return nullptr;
}

MonsterB* ClientObjectManager::GetMonsterB()
{
	int Size = (int)MonsterB_List.size();
	for (int i = 0; i < Size; i++)
	{
		if (MonsterB_List[i]->GetLife() == false)
		{
			MonsterB_List[i]->SetLife(true);
			MonsterB_List[i]->ReSet();
			return MonsterB_List[i];
		}
	}

	
	return nullptr;
}

Transform* ClientObjectManager::GetPlayerTransform()
{
	return PlayerObject->GetTransform();
}



void ClientObjectManager::CreateObjectMemorySize()
{
	///���� A �̸� �Ҵ�
	for (int i = 0; i < CreateMonsterACount; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterA_List.push_back(mFactory->CreateMonsterA(10+i, 0, -10));
	}

	///���� B �̸� �Ҵ�
	for (int i = 0; i < CreateMonsterBCount; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterB_List.push_back(mFactory->CreateMonsterB(0,5,-10));
	}

	///���� ��� �̸� �Ҵ�
	for (int i = 0; i < CreateAttackDroneCount; i++)
	{
		AttackDrone_List.push_back(mFactory->CreateAttackDrone(0 + i, 1, 0));
	}
	
	///Bullet �̸� �Ҵ�
	for (int i = 0; i < CreateBulletCount; i++)
	{
		Bullet_List.push_back(mFactory->CreateBullet(0, 4, 0));
	}

	///��Ż �̸� �Ҵ�
	int PotalPointSize = (int)PotalPoint_List.size();
	for (int i = 0; i < PotalPointSize; i++)
	{
		Vector3 Point = PotalPoint_List[i]->GetTransform()->Position;
		Potal_List.push_back(mFactory->CreatePortal(Point.x, Point.y, Point.z));
	}
}

void ClientObjectManager::OnActivePotal(bool isActive, int index)
{
	int Size = (int)Potal_List.size();
	if (index == -1) 
	{
		//��� ��Ż Ȱ��ȭ
		for (int i = 0; i < Size; i++)
		{
			Potal_List[i]->OnPotalActivation(isActive);
		}
	}
	else
	{
		if (Size - 1 >= index)
		{
			Potal_List[index]->OnPotalActivation(isActive);
		}
	}
}




