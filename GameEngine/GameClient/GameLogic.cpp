#include "GameLogic.h"
#include "MainHeader.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"
#include "ClientObjectManager.h"
#include "MonsterBase.h"

GameLogic::GameLogic()
{
	FactoryGM	= nullptr;
	ObjectGM	= nullptr;
}

GameLogic::~GameLogic()
{
	delete FactoryGM;
	delete ObjectGM;
}

void GameLogic::Initialize()
{
	//오브젝트 생성 매니저
	FactoryGM = new ObjectFactory();
	FactoryGM->Initialize();

	//오브젝트 관리 매니저
	ObjectGM = new ClientObjectManager();
	ObjectGM->Initialize(FactoryGM);
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{
	//CreateMonster(CreateMonsterMaxTime, PotalList[0]);
}

