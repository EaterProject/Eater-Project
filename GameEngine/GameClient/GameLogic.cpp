#include "GameLogic.h"
#include "EaterEngineAPI.h"
#include "ObjectFactory.h"
#include "MessageManager.h"


GameLogic::GameLogic()
{
	FactoryGM	= nullptr;
	ObjectGM	= nullptr;
}

GameLogic::~GameLogic()
{
	delete FactoryGM;
}

void GameLogic::Initialize()
{
	//�Ŵ��� ����
	FactoryGM	= new ObjectFactory();
	MessageManager::GetGM()->Initialize(FactoryGM);

}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}


