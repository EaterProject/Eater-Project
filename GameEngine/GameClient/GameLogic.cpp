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
	ObjectGM->GetGM()->Initialize(FactoryGM);
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{
	if (IsStart == false)
	{
		ObjectGM->GetGM()->SEND_Message(TARGET_GLOBAL,MESSAGE_GLOBAL_TITLE);
		IsStart = true;
	}


	//if (GetKeyDown(VK_F5))
	//{
	//	int num = 0;
	//	MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_OPEN,&num);
	//}
	//if (GetKeyDown(VK_F6))
	//{
	//	int num = 0;
	//	MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_CLOSE, &num);
	//}
	//
	//if (GetKeyDown(VK_F7))
	//{
	//	std::string Name = "StartCameraAnim";
	//	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_GAME_START, &Name);
	//}
	//
	//
	//if (GetKeyDown(VK_NUMPAD0))
	//{
	//	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CHANGE_PLAYER);
	//}
	//
	//if (GetKeyDown(VK_NUMPAD1))
	//{
	//	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CHANGE_DEBUG);
	//}
	//
	//
	//if (GetKeyDown(VK_NUMPAD2))
	//{
	//	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_IN);
	//}
	//
	//if (GetKeyDown(VK_NUMPAD3))
	//{
	//	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_FADE_OUT);
	//}
}


