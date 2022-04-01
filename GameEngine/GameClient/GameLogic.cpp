#include "GameLogic.h"
#include "MainHeader.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonsterManager.h"
#include "MonsterBase.h"

GameLogic::GameLogic()
{
	FactoryGM	= nullptr;
	MonsterGM	= nullptr;
}

GameLogic::~GameLogic()
{
	delete FactoryGM;
	delete MonsterGM;
}

void GameLogic::Initialize()
{
	//������Ʈ �Ŵ��� ����
	FactoryGM = new ObjectFactory();
	FactoryGM->Initialize();

	//���� �Ŵ��� ����
	MonsterGM = new MonsterManager();
	MonsterGM->Initialize();
	MonsterGM->SetFactory(FactoryGM);







	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("Potal", &PotalList);

	//�÷��̾� �浹�� ������Ʈ�� �����´�
	FindGameObjectTag("AttackCollider");
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{
	CreateMonster(CreateMonsterMaxTime, PotalList[0]);
}

void GameLogic::SetCreateMonsterMemorySize(int CreateCount)
{
	float X = 10;
	float Y = 0;
	float Z = 10;
	for (int i = 0; i < CreateCount; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterList.push_back(FactoryGM->CreateMonster(X,Y,Z+i,MONSTER_TYPE::MONSTER_A));
	}
}

MonsterBase* GameLogic::GetLifeMonter()
{
	//���� ������Ʈ ����Ʈ �߿��� �ൿ���� �ʴ� ���͸� ������
	int MonsterMaxCount = (int)MonsterList.size();
	for (int i = 0; i < MonsterMaxCount; i++)
	{
		if (MonsterList[i]->isLife == false)
		{
			return MonsterList[i];
		}
	}
	return nullptr;
}

void GameLogic::CreateMonster(float CreateMaxTime, GameObject* CreatePointObject)
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
		Transform* CreatePoint	= CreatePointObject->GetTransform();
		Transform* MonsterPoint = Monster->gameobject->GetTransform();
		MonsterPoint->Position	= CreatePoint->Position;
		CreateTime -= CreateMaxTime;
		Monster->isLife = true;
	}
}

