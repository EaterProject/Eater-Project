#include "MessageManager.h"

//���� ���۳�Ʈ
#include "Player.h"
#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ClientTypeOption.h"

//Ŭ���̾�Ʈ ���۳�Ʈ
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
	//���� ���丮 �ޱ�
	mFactory = Factory;

	//��Ż �±װ� �پ��ִ� ������Ʈ�� ��� ������ ����Ʈ�� ��Ƴ��´�
	FindGameObjectTags("ManaPoint", &mFactory->ManaPoint_List);
	
	////UI �����
	GameObject* Object = nullptr;

	//��Ʈ �̹��� ����
	Object			= mFactory->CreateFontImage();
	mFontImage		= Object->GetComponent<ComboFont>();

	//�÷��̾� ���� UI ����
	Object			= mFactory->CreatePlayerState();
	mPlayerState	= Object->GetComponent<PlayerStateImage>();

	//�÷��̾� ����
	Object			= mFactory->CreatePlayer();
	mPlayer			= Object->GetComponent<Player>();

	//������ ����
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
	case TARGET_PLAYER:	//�÷��̾�� �޼����� ������
		mPlayer->SetMessageRECV(MessageType, Data);
		break;
	case TARGET_DRONE:	//��п��� �޼����� ������
		break;
	case TARGET_BOSS:	//�������� �޼����� ������
		SEND_BOSS_Message(MessageType, Data);
		break;
	case TARGET_GLOBAL:	//�۷ι� �޼����� ������
		SEND_GLOBAL_Message(MessageType, Data);
		break;
	}
}

GameObject* MessageManager::CREATE_MESSAGE(int CREATE_TYPE)
{
	///Factory Ŭ������ ���� �޼����� ������

	switch (CREATE_TYPE)
	{
	case TARGET_PLAYER:	//�÷��̾�� �޼����� ������
		return mFactory->CreatePlayer();
	case TARGET_BOSS:	//�������� �޼����� ������
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







