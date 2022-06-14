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
	//매니저 생성
	FactoryGM	= new ObjectFactory();
	MessageManager::GetGM()->Initialize(FactoryGM);
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{
	if (GetKeyDown(VK_F5))
	{
		int num = 0;
		MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_OPEN,&num);
	}
	if (GetKeyDown(VK_F6))
	{
		int num = 0;
		MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_CLOSE, &num);
	}
}


