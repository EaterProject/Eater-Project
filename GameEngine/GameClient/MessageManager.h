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

	///��ü���� �޼����� ������ (���� ���, �޼��� Ÿ��, �޼��� ������)
	void SEND_Message(int Target, int MessageType, void* Data = nullptr);

	///Factory���� ���� �޼����� ������(������ ����� Target �޼���)
	GameObject* CREATE_MESSAGE(int CREATE_TYPE);

	///���ӻ� �Ѱ��� �����ϴ� ��ü�� ��������� �޼���
	GameObject* GET_MESSAGE(int GET_TYPE);
private:
	//Target �޼����� ���� ȣ��
	void SEND_Player_Message(int MessageType,void* Data);
	void SEND_UI_Message(int MessageType, void* Data);
	void SEND_BOSS_Message(int MessageType, void* Data);
	void SEND_GATE_Message(int MessageType, void* Data);
	void SEND_CAMERA_Message(int MessageType, void* Data);
	void SEND_GLOBAL_Message(int MessageType, void* Data);
private:
	std::vector<GameObject*>		ManaPoint_List;
private:
	Boss*				mBoss			= nullptr;
	GateDoor*			mGate			= nullptr;
	Player*				mPlayer			= nullptr;
	UICanvas*			mCanvas			= nullptr;
	CameraManager*		mCameraManager  = nullptr;
private:
	ObjectFactory* mFactory;
	friend ObjectFactory;
};



