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
class CameraManager;
class UIEffect;
class UITitle;
class UIStore;
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
	void SEND_BOSS_Message(int MessageType, void* Data);
	void SEND_GATE_Message(int MessageType, void* Data);
	void SEND_CAMERA_Message(int MessageType, void* Data);
	void SEND_GLOBAL_Message(int MessageType, void* Data);
private:
	void InGameStart();
	void TitleStart();
	void InGameEnd();
private:
	std::vector<GameObject*>		ManaPoint_List;
private:
	Boss*				mBoss			= nullptr;
	GateDoor*			mGate			= nullptr;
	Player*				mPlayer			= nullptr;
	UICanvas*			mCanvas			= nullptr;
	UIEffect*			mEffect			= nullptr;
	UITitle*			mTiltle			= nullptr;
	UIStore*			mStore			= nullptr;
	CameraManager*		mCameraManager  = nullptr;
private:
	ObjectFactory* mFactory;
	friend ObjectFactory;
};



