#include "GameLogic.h"
#include "MainHeader.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonsterBase.h"

GameLogic::GameLogic()
{
	Factory = nullptr;
	CreateMonsterTime = 0.0f;
}

GameLogic::~GameLogic()
{

}

void GameLogic::Initialize()
{
	//������Ʈ �Ŵ��� ����
	Factory = new ObjectFactory();
	Factory->Initialize();

	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("Potal", &PotalList);

	//���͸� �̸� �Ҵ�
	SetCreateMonsterMemorySize(10);
}

void GameLogic::Release()
{
	Factory->Release();
	delete Factory;
}

void GameLogic::Update()
{
	CreateMonsterTime += GetDeltaTime();
	if (CreateMonsterTime >= CreateMonsterMaxTime)
	{
		LifeMonsterCheck();

		CreateMonsterTime -= CreateMonsterMaxTime;
	}
}

void GameLogic::SetCreateMonsterMemorySize(int CreateCount)
{
	float X = 10;
	float Y = 0;
	float Z = 10;
	for (int i = 0; i < CreateCount; i++)
	{
		//������ ���͸� ����Ʈ�� ��´�
		MonsterList.push_back(Factory->CreateMonster(X,Y,Z+i,MONSTER_TYPE::MONSTER_A));
	}
}

void GameLogic::LifeMonsterCheck()
{
	int MonsterMaxCount = (int)MonsterList.size();
	for (int i = 0; i < MonsterMaxCount; i++)
	{
		if (MonsterList[i]->isLife == false)
		{
			 GameObject* Object = MonsterList[i]->gameobject;
			Object->GetTransform()->Position  = (PotalList[0]->GetTransform()->Position);
			MonsterList[i]->isLife = true;
			return;
		}
	}
}

