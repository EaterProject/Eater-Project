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
#include "Monster.h"


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

GameObject* ObjectFactory::CreateMonster(float x, float y, float z)
{
	GameObject* Object_Monster = Instance();
 	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();
	Object_Monster->AddComponent<Monster>();
	Object_Monster->GetTransform()->Position = { x,y,z };
	return Object_Monster;
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
