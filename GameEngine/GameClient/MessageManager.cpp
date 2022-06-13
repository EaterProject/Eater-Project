#include "MessageManager.h"

//엔진 컨퍼넌트
#include "Player.h"
#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ClientTypeOption.h"

//클라이언트 컨퍼넌트
#include "PlayerCamera.h"
#include "ClientComponent.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "Drone.h"
#include "Bullet.h"
#include "ComboFont.h"
#include "PlayerStateImage.h"


MessageManager* MessageManager::instance = nullptr;
MessageManager::MessageManager()
{
	mFactory		= nullptr;
	mPlayer			= nullptr;
}

MessageManager::~MessageManager()
{
	mFactory		= nullptr;
	mPlayer			= nullptr;
}

MessageManager* MessageManager::GetGM()
{
	if (instance == nullptr)
	{
		instance = new MessageManager();
		return instance;
	}
	else
	{
		return instance;
	}

}

void MessageManager::Initialize(ObjectFactory* Factory)
{
	//생성 펙토리 받기
	mFactory = Factory;

	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	FindGameObjectTags("ManaPoint", &mFactory->ManaPoint_List);
	
	////UI 만들기
	GameObject* Object = nullptr;

	//폰트 이미지 생성
	Object			= mFactory->CreateFontImage();
	mFontImage		= Object->GetComponent<ComboFont>();

	//플레이어 상태 UI 생성
	Object			= mFactory->CreatePlayerState();
	mPlayerState	= Object->GetComponent<PlayerStateImage>();

	//플레이어 생성
	Object			= mFactory->CreatePlayer();
	mPlayer			= Object->GetComponent<Player>();

	//마나석 생성
	Object			= mFactory->CreateManaStone();
}

void MessageManager::Release()
{
	mFactory = nullptr;
}

void MessageManager::SEND_Message(int Target, int MessageType, void* Data)
{
	switch (Target)
	{
	case TARGET_PLAYER:	//플레이어에게 메세지를 보낸다
		mPlayer->SetMessageRECV(MessageType, Data);
		break;
	case TARGET_DRONE:	//드론에게 메세지를 보낸다
		break;
	case TARGET_BOSS:	//보스에게 메세지를 보낸다
		SEND_BOSS_Message(MessageType, Data);
		break;
	case TARGET_GLOBAL:	//글로벌 메세지를 보낸다
		SEND_GLOBAL_Message(MessageType, Data);
		break;
	}
}

GameObject* MessageManager::CREATE_MESSAGE(int CREATE_TYPE)
{
	///Factory 클래스에 생성 메세지를 보낸다

	switch (CREATE_TYPE)
	{
	case TARGET_PLAYER:	//플레이어에게 메세지를 보낸다
		return mFactory->CreatePlayer();
	case TARGET_BOSS:	//보스에게 메세지를 보낸다
		break;
	case TARGET_MANA:
		return mFactory->CreateManaStone();
	case TARGET_MONSTER_A:
		return mFactory->CreateMonsterA();
	case TARGET_MONSTER_B:
		return mFactory->CreateMonsterB();
	}

	return nullptr;
}

void MessageManager::SEND_Player_Message(int MessageType, void* Data)
{
	mPlayer->SetMessageRECV(MessageType, Data);
}

void MessageManager::SEND_UI_Message(int MessageType, void* Data)
{


}

void MessageManager::SEND_BOSS_Message(int MessageType, void* Data)
{


}

void MessageManager::SEND_GLOBAL_Message(int MessageType, void* Data)
{
	switch (MessageType)
	{
	case MESSAGE_GLOBAL_COMBO:
		mFontImage->SetComboNumber(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_GLOBAL_HP:
	{
		mPlayerState->SetHP(*(reinterpret_cast<int*>(Data)));
	}
		break;
	case MESSAGE_GLOBAL_CHANGE:
		mPlayerState->SetChangeCount(*(reinterpret_cast<int*>(Data)));
		break;
	case 3:
		break;

	}



}







