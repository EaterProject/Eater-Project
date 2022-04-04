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
	//������Ʈ ���� �Ŵ���
	FactoryGM = new ObjectFactory();
	FactoryGM->Initialize();

	//������Ʈ ���� �Ŵ���
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

