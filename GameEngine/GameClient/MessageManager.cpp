#include "MessageManager.h"

//���� ���۳�Ʈ
#include "Player.h"
#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ClientTypeOption.h"
#include <string>

//Ŭ���̾�Ʈ ���۳�Ʈ
#include "PlayerCamera.h"
#include "ClientComponent.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "Drone.h"
#include "Bullet.h"
#include "ComboFont.h"
#include "UICanvas.h"
#include "GateDoor.h"
#include "Boss.h"
#include "CameraManager.h"
#include "ManaStone.h"

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
	//���� ���丮 �ޱ�
	mFactory = Factory;

	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("ManaPoint", &mFactory->ManaPoint_List);
	

	//�÷��̾� ����
	CREATE_MESSAGE(TARGET_CAMERA_MANAGER);
	CREATE_MESSAGE(TARGET_GATE_MANAGER);
	CREATE_MESSAGE(TARGET_UI);
	CREATE_MESSAGE(TARGET_PLAYER);


	CREATE_MESSAGE(TARGET_MANA);
	CREATE_MESSAGE(TARGET_BOSS);
}

void MessageManager::Release()
{
	mFactory = nullptr;
}

void MessageManager::SEND_Message(int Target, int MessageType, void* Data)
{
	switch (Target)
	{
	case TARGET_PLAYER:	//�÷��̾�� �޼����� ������
		mPlayer->SetMessageRECV(MessageType, Data);
		break;
	case TARGET_DRONE:	//��п��� �޼����� ������
		break;
	case TARGET_BOSS:	//�������� �޼����� ������
		SEND_BOSS_Message(MessageType, Data);
		break;
	case TARGET_UI:	//�۷ι� �޼����� ������
		SEND_UI_Message(MessageType, Data);
		break;
	case TARGET_GATE_MANAGER:	//�۷ι� �޼����� ������
		SEND_GATE_Message(MessageType, Data);
		break;
	case TARGET_CAMERA_MANAGER:
		SEND_CAMERA_Message(MessageType, Data);
		break;
	case TARGET_GLOBAL:
		SEND_GLOBAL_Message(MessageType, Data);
		break;
	}
}

GameObject* MessageManager::CREATE_MESSAGE(int CREATE_TYPE)
{
	///Factory Ŭ������ ���� �޼����� ������
	GameObject* Object = nullptr;

	switch (CREATE_TYPE)
	{
	case TARGET_PLAYER:	//�÷��̾�� �޼����� ������
		Object	= mFactory->CreatePlayer();
		mPlayer = Object->GetComponent<Player>();
		return Object;
	case TARGET_BOSS:	//�������� �޼����� ������
		Object = mFactory->CreateBoss();
		mBoss = Object->GetComponent<Boss>();
		return Object;
	case TARGET_MANA:
		return mFactory->CreateManaStone();
	case TARGET_MONSTER_A:
		return mFactory->CreateMonsterA();
	case TARGET_MONSTER_B:
		return mFactory->CreateMonsterB();
	case TARGET_GATE_IN:
		return mFactory->CreateGate_In();
	case TARGET_GATE_OUT:
		return mFactory->CreateGate_Out();
	case TARGET_GATE_MANAGER:
		Object	= mFactory->CreateGate_Manager();
		mGate	= Object->GetComponent<GateDoor>();
		return Object;
	case TARGET_CAMERA_MANAGER:
		Object = mFactory->CreateCameraManager();
		mCameraManager = Object->GetComponent<CameraManager>();
		return Object;
	case TARGET_UI:
		Object = mFactory->CreateUICanvas();
		mCanvas = Object->GetComponent<UICanvas>();
		return Object;
	}

	return nullptr;
}

GameObject* MessageManager::GET_MESSAGE(int GET_TYPE)
{
	
	return nullptr;
}

void MessageManager::SEND_Player_Message(int MessageType, void* Data)
{
	///Boss �޼��� ����
	mPlayer->SetMessageRECV(MessageType, Data);
}

void MessageManager::SEND_BOSS_Message(int MessageType, void* Data)
{
	///Boss �޼��� ����

}

void MessageManager::SEND_UI_Message(int MessageType, void* Data)
{
	///UI �޼��� ����

	switch (MessageType)
	{
	case MESSAGE_UI_COMBO:
		mCanvas->Set_Combo_Now(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_HP_NOW:
		mCanvas->Set_HP_Now(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_HP_MAX:
		mCanvas->Set_HP_Max(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_EMAGIN_NOW:
		mCanvas->Set_Emagin_Now(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_EMAGIN_MAX:
		mCanvas->Set_Emagin_Max(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_MONSTER_UI_ON:
		mCanvas->Set_Monster_EMAGINE(Data);
		break;
	case MESSAGE_UI_RENDER:
		mCanvas->Set_ALLRender(*(reinterpret_cast<int*>(Data)));
		break;
	}
}

void MessageManager::SEND_GATE_Message(int MessageType, void* Data)
{
	///����Ʈ �޼��� ����

	switch (MessageType)
	{
	case MESSAGE_GATE_OPEN:
		mGate->SetOpen(*reinterpret_cast<int*>(Data));
		break;
	case MESSAGE_GATE_CLOSE:
		mGate->SetClose(*reinterpret_cast<int*>(Data));
		break;
	}
}

void MessageManager::SEND_CAMERA_Message(int MessageType, void* Data)
{
	///ī�޶� �޼��� ����

	switch (MessageType)
	{
	case MESSAGE_CAMERA_CINEMATIC_GAME_START:
	case MESSAGE_CAMERA_CINEMATIC_GAME_END:
	case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
	case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
		mCameraManager->SetCinematic(MessageType,*(reinterpret_cast<std::string*>(Data)));
		break;
	case MESSAGE_CAMERA_CHANGE_DEBUG:
	case MESSAGE_CAMERA_CHANGE_PLAYER:
		mCameraManager->Change(MessageType);
		break;
	}
}

void MessageManager::SEND_GLOBAL_Message(int MessageType, void* Data)
{
	switch (MessageType)
	{
	case MESSAGE_GLOBAL_GAMESTART:	//���� ����
		//CREATE_MESSAGE(TARGET_UI);
		//CREATE_MESSAGE(TARGET_PLAYER);
		//SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CHANGE_PLAYER);
		break;
	case MESSAGE_GLOBAL_GAMEEND:	//���� ����
		break;
	}




}







