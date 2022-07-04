#pragma once
#include <vector>
#include "ClientTypeOption.h"

class ClientComponent;
class ObjectFactory;
class GameObject;
class Transform;

class Player;
class Drone;
class AttackDrone;
class Bullet;
class MonsterA;
class MonsterB;
class FontImage;
class ComboFont;
class UICanvas;
class GateDoor;
class Boss;
class Store;
class CameraManager;
class SceneEffect;
class UIEffect;
class UITitle;
class UIStore;
class UIOption;
class UIPause;
class UIManual;
class UICredit;
class UIBoss;

enum class  CLIENT_OBJECT_TYPE
{
	MONATER_A,
	MONATER_B,
	ATTACk_DRONE,
	POTAL,
	BULLET,
};

class MessageManager
{
private:
	MessageManager();
	~MessageManager();
	static MessageManager* instance;
public:
	static MessageManager* GetGM();

	void Initialize(ObjectFactory* Factory);
	void Release();

	///객체에게 메세지를 보낸다 (보낼 대상, 메세지 타입, 메세지 데이터)
	void SEND_Message(int Target, int MessageType, void* Data = nullptr);

	///Factory에게 생성 메세지를 보낸다(생성할 대상의 Target 메세지)
	GameObject* CREATE_MESSAGE(int CREATE_TYPE);
private:
	//Target 메세지에 따른 호출
	void SEND_Player_Message(int MessageType,void* Data);
	void SEND_UI_Message(int MessageType, void* Data);
	void SEND_DRONE_Message(int MessageType, void* Data);
	void SEND_BOSS_Message(int MessageType, void* Data);
	void SEND_GATE_Message(int MessageType, void* Data);
	void SEND_CAMERA_Message(int MessageType, void* Data);
	void SEND_GLOBAL_Message(int MessageType, void* Data);
private:
	void InGameStart();
	void TitleStart();
	void OptionStart(int prev_state);
	void PauseStart();
	void InGameResume();
	void InGameEnd();
	void ManualStart();
	void StoreStart();
	void CreditStart();

private:
	std::vector<GameObject*>		ManaPoint_List;
private:
	Boss*				mBoss			= nullptr;
	GateDoor*			mGate			= nullptr;
	Player*				mPlayer			= nullptr;
	Store*				mStoreMachine	= nullptr;
	SceneEffect*		mSceneEffect	= nullptr;
	UICanvas*			mCanvas			= nullptr;
	UIEffect*			mEffect			= nullptr;
	UITitle*			mTiltle			= nullptr;
	UIStore*			mStore			= nullptr;
	UIOption*			mOption			= nullptr;
	UIPause*			mPause			= nullptr;
	UIManual*			mManual			= nullptr;
	UICredit*			mCredit			= nullptr;
	UIBoss*				mBossUI			= nullptr;
	CameraManager*		mCameraManager  = nullptr;
private:
	ObjectFactory* mFactory;
	friend ObjectFactory;
};



