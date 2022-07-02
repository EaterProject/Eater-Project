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
#include "EaterEngineAPI.h"
#include "MiniMapSystem.h"

//Ŭ���̾�Ʈ ���۳�Ʈ
#include "PlayerCamera.h"
#include "ClientComponent.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "Drone.h"
#include "ComboFont.h"
#include "UICanvas.h"
#include "GateDoor.h"
#include "Boss.h"
#include "CameraManager.h"
#include "ManaStone.h"
#include "UIEffect.h"
#include "UITitle.h"
#include "UIStore.h"
#include "UIOption.h"
#include "UIPause.h"
#include "UIBoss.h"

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

	//�̴ϸ� ����
	MiniMapSystem* m_MiniMap = MiniMapSystem::Get();
	m_MiniMap->CreateMiniMap("ingame_minimap", PIVOT_RIGHT_TOP, ROTATE_90, Vector2(186.0f), Vector2(-25.0f));

	//�÷��̾� ����
	CREATE_MESSAGE(TARGET_CAMERA_MANAGER);
	CREATE_MESSAGE(TARGET_GATE_MANAGER);
	CREATE_MESSAGE(TARGET_UI);
	CREATE_MESSAGE(TARGET_UI_EFFECT);
	CREATE_MESSAGE(TARGET_UI_TITLE);
	CREATE_MESSAGE(TARGET_UI_STORE);
	CREATE_MESSAGE(TARGET_PLAYER);
	CREATE_MESSAGE(TARGET_STORE);
	CREATE_MESSAGE(TARGET_MANA);
	CREATE_MESSAGE(TARGET_BOSS);
	CREATE_MESSAGE(TARGET_UI_OPTION);
	CREATE_MESSAGE(TARGET_UI_PAUSE);
	CREATE_MESSAGE(TARGET_UI_BOSS);
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
	case TARGET_BOSS_WEAPON:
		return mFactory->CreateBossWeapon();
	case TARGET_BOSS_FRIEND:
		return mFactory->CreateBossFriend();
	case TARGET_UI_EFFECT:
		Object = mFactory->CreateUIEffect();
		mEffect = Object->GetComponent<UIEffect>();
		return Object;
	case TARGET_UI_TITLE:
		Object = mFactory->CreateUITitle();
		mTiltle = Object->GetComponent<UITitle>();
		return Object;
	case TARGET_UI_STORE:
		Object = mFactory->CreateUIStore();
		mStore = Object->GetComponent<UIStore>();
		return Object;
	case TARGET_STORE:
		return  mFactory->CreateStore();
	case TARGET_UI_OPTION:
		Object = mFactory->CreateUIOption();
		mOption = Object->GetComponent<UIOption>();
		return Object;
	case TARGET_UI_PAUSE:
		Object = mFactory->CreateUIPause();
		mPause = Object->GetComponent<UIPause>();
		return Object;
	case TARGET_UI_BOSS:
		Object = mFactory->CreateUIBoss();
		mBossUI = Object->GetComponent<UIBoss>();
		return Object;
	}

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
	//��� UI�޼����� �������� ����

	switch (MessageType)
	{
	case MESSAGE_UI_COMBO:
		mCanvas->Set_Combo_Now(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_HP_NOW:
		mCanvas->Set_Player_HP(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_HP_MAX:
		mCanvas->Set_Player_HP_Max(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_EMAGIN_NOW:
		mCanvas->Set_Player_Emagin(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_EMAGIN_MAX:
		mCanvas->Set_Player_Emagin_Max(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_MONSTER_UI_ON:
		mCanvas->Set_Monster_UI_ON(Data);
		break;
	case MESSAGE_UI_MONSTER_UI_OFF:
		mCanvas->Set_Monster_UI_OFF(Data);
		break;
	case MESSAGE_UI_MONSTER_UI_UPDATE:
		mCanvas->Set_Monster_UI_SET_DATA(Data);
		break;
	case MESSAGE_UI_FADE_IN:
		mEffect->Fade_IN(Data);
		break;
	case MESSAGE_UI_FADE_OUT:
		mEffect->Fade_OUT(Data);
		break;
	case MESSAGE_UI_STORE_ACTIVE:
		mStore->Set_Store_Active(*(reinterpret_cast<bool*>(Data)));
		break;
	case MESSAGE_UI_PLAYER_ACTIVE:
		mCanvas->Set_InGameUI_Active(*(reinterpret_cast<bool*>(Data)));
		break;
	case MESSAGE_UI_PLAYER_HIT:
		mEffect->PlayerHit(nullptr);
		break;
	case MESSAGE_UI_BOSS_ACTIVE:
		mBossUI->SetActive(*(reinterpret_cast<bool*>(Data)));
		break;
	case MESSAGE_UI_BOSS_HP:
		mBossUI->SetBossHP(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_BOSS_HP_MAX:
		mBossUI->SetBossMaxHP(*(reinterpret_cast<int*>(Data)));
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
	case MESSAGE_GATE_LOCK:
		mGate->SetLock(*reinterpret_cast<int*>(Data));
		break;
	case MESSAGE_GATE_UNLOCK:
		mGate->SetUnLock(*reinterpret_cast<int*>(Data));
		break;
	}
}

void MessageManager::SEND_CAMERA_Message(int MessageType, void* Data)
{
	///ī�޶� �޼��� ����
	switch (MessageType)
	{
	case MESSAGE_CAMERA_CINEMATIC_GAME_START:
		break;
	case MESSAGE_CAMERA_CINEMATIC_GAME_END:
		break;
	case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
		break;
	case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
		//mCameraManager->SetCinematic(MessageType,*(reinterpret_cast<std::string*>(Data)));
		break;
	case MESSAGE_CAMERA_CINEMATIC_TITLE:
		mCameraManager->SetCinematic(MessageType, *(reinterpret_cast<std::string*>(Data)));
		break;
	case MESSAGE_CAMERA_CHANGE_DEBUG:
		break;
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
		InGameStart();
		break;
	case MESSAGE_GLOBAL_GAMEEND:	//���� ����
		InGameEnd();
		break;
	case MESSAGE_GLOBAL_TITLE:		//���� ����
		TitleStart();
		break;
	case MESSAGE_GLOBAL_OPTION:		//�ɼ�
		OptionStart(*(int*)(Data));
		break;
	case MESSAGE_GLOBAL_RESUME:		//�Ͻ�����
		break;
	}
}

void MessageManager::InGameStart()
{
	mTiltle->SetTitleUIActive(false);
	mCanvas->Set_InGameUI_Active(true);
	mOption->SetOptionUIActive(false);
	mPause->SetPauseUIActive(false);

	//ī�޶�,�÷��̾� ����
	SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ACTIVE_TRUE);
	SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CHANGE_PLAYER);

	//���� ���
	Sound_Play_BGM("InGame_OutDoor");
}

void MessageManager::TitleStart()
{
	mTiltle->SetTitleUIActive(true);
	mCanvas->Set_InGameUI_Active(false);
	mOption->SetOptionUIActive(false);
	mPause->SetPauseUIActive(false);


	//SEND_Message(TARGET_UI,)
	//SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ACTIVE_FALSE);

	//Sound_Play_BGM("Title");
}

void MessageManager::OptionStart(int prev_state)
{
	mTiltle->SetTitleUIActive(false);
	mCanvas->Set_InGameUI_Active(false);
	mPause->SetPauseUIActive(false);
	mOption->SetOptionUIActive(true);
	mOption->SetPrevTarget(prev_state);
}

void MessageManager::PauseStart()
{
	mTiltle->SetTitleUIActive(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->SetOptionUIActive(false);
	mPause->SetPauseUIActive(true);
}

void MessageManager::InGameEnd()
{




	// ������ ����..
	PostQuitMessage(WM_QUIT);
}







