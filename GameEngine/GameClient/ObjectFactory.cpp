#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"

//Ŭ���̾�Ʈ�� ���۳�Ʈ
#include "Player.h"
#include "PlayerCamera.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "MonsterBase.h"


ObjectFactory::ObjectFactory()
{
	PlayerObject	= nullptr;
	PlayerMainCamera = nullptr;
}

ObjectFactory::~ObjectFactory()
{
	
}

void ObjectFactory::Initialize()
{
	//������ �������� �÷��̾�, ī�޶� ������
	PlayerObject		= FindGameObjectTag("Player");
	PlayerMainCamera	= FindGameObjectTag("MainCam");

	//Ŭ���̾�Ʈ�� ���۳�Ʈ Add
	PlayerObject->AddComponent<Player>();
	PlayerMainCamera->GetComponent<Camera>()->ChoiceMainCam();
	PlayerMainCamera->AddComponent<PlayerCamera>();
}

void ObjectFactory::Release()
{





}

GameObject* ObjectFactory::CreateCameraPlayer()
{
	return nullptr;
}

GameObject* ObjectFactory::CreatePlayer()
{
	return nullptr;
}

MonsterBase* ObjectFactory::CreateMonster(float x, float y, float z,MONSTER_TYPE Type)
{
	GameObject* Object_Monster = Instance();
 	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();
	MonsterBase* Base = Object_Monster->AddComponent<MonsterBase>();
	switch (Type)
	{
	case MONSTER_TYPE::MONSTER_A:
		Object_Monster->AddComponent<MonsterA>();
		break;
	case MONSTER_TYPE::MONSTER_B:
		Object_Monster->AddComponent<MonsterB>();
		break;
	}
	Object_Monster->GetTransform()->Position = { x,y,z };
	return Base;
}

GameObject* ObjectFactory::CreateMana()
{
	return nullptr;
}

GameObject* ObjectFactory::CreateDrone()
{
	return nullptr;
}

GameObject* ObjectFactory::CreatePortal()
{
	return nullptr;
}
