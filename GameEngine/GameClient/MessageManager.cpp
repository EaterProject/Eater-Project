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
#include "Store.h"
#include "CameraManager.h"
#include "ManaStone.h"
#include "SceneEffect.h"
#include "UIEffect.h"
#include "UITitle.h"
#include "UIStore.h"
#include "UIOption.h"
#include "UIPause.h"
#include "UIManual.h"
#include "UICredit.h"
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
	CREATE_MESSAGE(TARGET_SCENE_EFFECT);
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
	CREATE_MESSAGE(TARGET_UI_MANUAL);
	CREATE_MESSAGE(TARGET_UI_CREDIT);
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
		SEND_DRONE_Message(MessageType, Data);
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
		Object = mFactory->CreateGate_Manager();
		mGate = Object->GetComponent<GateDoor>();
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
		Object = mFactory->CreateStore();
		mStoreMachine = Object->GetComponent<Store>();
		return Object;
	case TARGET_UI_OPTION:
		Object = mFactory->CreateUIOption();
		mOption = Object->GetComponent<UIOption>();
		return Object;
	case TARGET_UI_PAUSE:
		Object = mFactory->CreateUIPause();
		mPause = Object->GetComponent<UIPause>();
		return Object;
	case TARGET_UI_MANUAL:
		Object = mFactory->CreateUIManual();
		mManual = Object->GetComponent<UIManual>();
		return Object;
	case TARGET_UI_CREDIT:
		Object = mFactory->CreateUICredit();
		mCredit = Object->GetComponent<UICredit>();
		return Object;
	case TARGET_UI_BOSS:
		Object = mFactory->CreateUIBoss();
		mBossUI = Object->GetComponent<UIBoss>();
		return Object;
	case TARGET_SCENE_EFFECT:
		Object = mFactory->CreateSceneEffect();
		mSceneEffect = Object->GetComponent<SceneEffect>();
		return Object;
	}

	return nullptr;
}

void MessageManager::SEND_Player_Message(int MessageType, void* Data)
{
	///Player �޼��� ����
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
	case MESSAGE_UI_PLAYER_ACTIVE:
		mCanvas->Set_InGameUI_Active(*(reinterpret_cast<bool*>(Data)));
		break;
	case MESSAGE_UI_PLAYER_HIT:
		mEffect->PlayerHit(nullptr);
		break;
	case MESSAGE_UI_BOSS_HP:
		mBossUI->SetBossHP(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_BOSS_HP_MAX:
		mBossUI->SetBossMaxHP(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_BOSS_ACTIVE:
		mBossUI->SetBossUIActive(*(reinterpret_cast<bool*>(Data)));
		break;
	case MESSAGE_UI_PUREMANA:
		mCanvas->Set_Pure_Mana_Count(*(reinterpret_cast<int*>(Data)));
		mStore->Set_Pure_Mana_Count(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_COREMANA:
		mCanvas->Set_Core_Mana_Count(*(reinterpret_cast<int*>(Data)));
		mStore->Set_Core_Mana_Count(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_UI_SKILL_E:
		mCanvas->Set_Skill_E(*(reinterpret_cast<float*>(Data)));
		break;
	case MESSAGE_UI_SKILL_MR:
		mCanvas->Set_Skill_MR(*(reinterpret_cast<float*>(Data)));
		break;
	case MESSAGE_UI_SKILL_SPC:
		mCanvas->Set_Skill_SPC(*(reinterpret_cast<float*>(Data)));
		break;
	}
}

void MessageManager::SEND_DRONE_Message(int MessageType, void* Data)
{
	switch (MessageType)
	{
	case MESSAGE_DRONE_GAME_START:			// O
		mCanvas->Push_Game_Start_Text();
		break;
	case MESSAGE_DRONE_DOOM_OUT:			// O
		mCanvas->Push_Doom_Out_Text();
		break;
	case MESSAGE_DRONE_STORE:				// O
		mCanvas->Push_Store_Text();
		break;
	case MESSAGE_DRONE_PURCHASE_SUCCESS:	// O
		mCanvas->Push_Purchase_Success_Text();
		break;
	case MESSAGE_DRONE_PURCHASE_FAIL:		// O
		mCanvas->Push_Purchase_Fail_Text();
		break;
	case MESSAGE_DRONE_BOSS_START:			// �ؾߵ�
		mCanvas->Push_Boss_Start_Text();
		break;
	case MESSAGE_DRONE_BOSS_ZONE_IN:		// O
		mCanvas->Push_Boss_Zone_In_Text();
		break;
	case MESSAGE_DRONE_MANA_CREATE:			// O
		mCanvas->Push_Mana_Create_Text();
		break;
	case MESSAGE_DRONE_GET_PUREMANA:		// O
		mCanvas->Push_Get_PureMana_Text();
		break;
	case MESSAGE_DRONE_GET_COREMANA:		// O
		mCanvas->Push_Get_CoreMana_Text();
		break;
	case MESSAGE_DRONE_PLAYER_DIE:			// O
		mCanvas->Push_Player_Die_Text();
		break;
	case MESSAGE_DRONE_PLAYER_HEAL:			// O
		mCanvas->Push_Player_Heal_Text();
		break;
	default:
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
	case MESSAGE_CAMERA_CINEMATIC:
		mCameraManager->SetCinematic(MessageType, *(reinterpret_cast<std::string*>(Data)));
		break;
	case MESSAGE_CAMERA_CINEMATIC_GAME_START:
	case MESSAGE_CAMERA_CINEMATIC_GAME_END:
	case MESSAGE_CAMERA_CINEMATIC_BOSS_START:
	case MESSAGE_CAMERA_CINEMATIC_BOSS_END:
	case MESSAGE_CAMERA_CINEMATIC_TITLE:
		mCameraManager->SetCinematic(MessageType, reinterpret_cast<bool*>(Data));
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
	case MESSAGE_GLOBAL_GAME_START:	//���� ����
		InGameStart();
		break;
	case MESSAGE_GLOBAL_GAME_END:	//���� ����
		InGameEnd();
		break;
	case MESSAGE_GLOBAL_BOSS_START:	//���� ����
		BossStart();
		break;
	case MESSAGE_GLOBAL_TITLE:		//���� ����
		TitleStart();
		break;
	case MESSAGE_GLOBAL_OPTION:		//�ɼ�
		OptionStart(*reinterpret_cast<int*>(Data));
		break;
	case MESSAGE_GLOBAL_PAUSE:		//�Ͻ�����
		PauseStart();
		break;
	case MESSAGE_GLOBAL_RESUME:		//���� ����
		InGameResume();
		break;
	case MESSAGE_GLOBAL_MANUAL:		//�޴���
		ManualStart();
		break;
	case MESSAGE_GLOBAL_STORE:		//����
		StoreStart();
		break;
	case MESSAGE_GLOBAL_CREDIT:		//ũ����
		CreditStart();
		break;
	}
}

void MessageManager::InGameStart()
{
	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(true);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	//ī�޶�,�÷��̾� ����
	SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ACTIVE_TRUE);
	SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CHANGE_PLAYER);

	//��� �ؽ�Ʈ ���
	SEND_Message(TARGET_DRONE, MESSAGE_DRONE_GAME_START);
	SEND_Message(TARGET_DRONE, MESSAGE_DRONE_MANA_CREATE);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(true);

	// ���콺 ����..
	mCameraManager->SetMouseFix(true);

	//���� ���
	Sound_Play_BGM("InGame_OutDoor");
}

void MessageManager::TitleStart()
{
	mTiltle->Set_TitleUI_Active(true);

	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);

	//SEND_Message(TARGET_UI,)
	//SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ACTIVE_FALSE);

	Sound_Play_BGM("Title");
}

void MessageManager::OptionStart(int prev_state)
{
	mOption->Set_OptionUI_Active(true);

	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	// ���� ���� ����..
	mOption->SetPrevTarget(prev_state);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);
}

void MessageManager::PauseStart()
{
	mPause->Set_PauseUI_Active(true);

	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);
	mBossUI->Set_BossUI_Draw(false);

	// ���� ��ȣ�ۿ� ��Ȱ��ȭ..
	mStoreMachine->SetStoreActive(false);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);
}

void MessageManager::InGameResume()
{
	mCanvas->Set_InGameUI_Active(true);
	mBossUI->Set_BossUI_Draw(true);

	mTiltle->Set_TitleUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	// ��� �ؽ�Ʈ �ǹ� ��ġ ��ü
	mCanvas->Set_Drone_Text_Pivot(PIVOT_TYPE::PIVOT_MIDDLE_LEFT);

	// ���� ��ȣ�ۿ� Ȱ��ȭ..
	mStoreMachine->SetStoreActive(true);

	// �÷��̾� ���� ���� ����..
	mPlayer->SetNoHit(false);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(true);

	// ���콺 ����..
	mCameraManager->SetMouseFix(true);
}

void MessageManager::InGameEnd()
{




	// ������ ����..
	PostQuitMessage(WM_QUIT);
}

void MessageManager::BossStart()
{
	// ���� ���� ����Ʈ ����..
	mSceneEffect->Begin_Boss_Start_Effect();

	mManual->Set_ManualUI_Active(false);
	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	// �÷��̾� ���� ���� ����..
	mPlayer->SetNoHit(true);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(true);
}

void MessageManager::ManualStart()
{
	mManual->Set_ManualUI_Active(true);

	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mCredit->Set_CreditUI_Active(false);
	mStore->Set_Store_Active(false);

	// ���� ��ȣ�ۿ� ��Ȱ��ȭ..
	mStoreMachine->SetStoreActive(false);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);
}

void MessageManager::StoreStart()
{
	mStore->Set_Store_Active(true);

	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mCredit->Set_CreditUI_Active(false);

	// ��� �ؽ�Ʈ �ǹ� ��ġ ��ü
	mCanvas->Set_Drone_Text_Pivot(PIVOT_TYPE::PIVOT_MIDDLE_RIGHT);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);
}

void MessageManager::CreditStart()
{
	mCredit->Set_CreditUI_Active(true);

	mTiltle->Set_TitleUI_Active(false);
	mCanvas->Set_InGameUI_Active(false);
	mOption->Set_OptionUI_Active(false);
	mPause->Set_PauseUI_Active(false);
	mManual->Set_ManualUI_Active(false);
	mStore->Set_Store_Active(false);

	// �÷��̾� Ű���� ����..
	mPlayer->SetKeyState(false);

	// ���콺 ����..
	mCameraManager->SetMouseFix(false);
}
