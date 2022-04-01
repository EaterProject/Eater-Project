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
	//오브젝트 매니저 생성
	FactoryGM = new ObjectFactory();
	FactoryGM->Initialize();

	//몬스터 매니저 생성
	MonsterGM = new MonsterManager();
	MonsterGM->Initialize();
	MonsterGM->SetFactory(FactoryGM);







	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	FindGameObjectTags("Potal", &PotalList);

	//플레이어 충돌용 오브젝트를 가져온다
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
		//생성한 몬스터를 리스트에 담는다
		MonsterList.push_back(FactoryGM->CreateMonster(X,Y,Z+i,MONSTER_TYPE::MONSTER_A));
	}
}

MonsterBase* GameLogic::GetLifeMonter()
{
	//몬스터 오브젝트 리스트 중에서 행동하지 않는 몬스터를 가져옴
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
		Transform* CreatePoint	= CreatePointObject->GetTransform();
		Transform* MonsterPoint = Monster->gameobject->GetTransform();
		MonsterPoint->Position	= CreatePoint->Position;
		CreateTime -= CreateMaxTime;
		Monster->isLife = true;
	}
}

