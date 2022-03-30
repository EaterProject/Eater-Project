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
	//오브젝트 매니저 생성
	Factory = new ObjectFactory();
	Factory->Initialize();

	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	FindGameObjectTags("Potal", &PotalList);

	//몬스터를 미리 할당
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
		//생성한 몬스터를 리스트에 담는다
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

