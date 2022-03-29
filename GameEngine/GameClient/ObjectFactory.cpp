#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"

//Ŭ���̾�Ʈ�� ���۳�Ʈ
#include "Player.h"
#include "PlayerCamera.h"
#include "Camera.h"


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

GameObject* ObjectFactory::CreateMonster()
{
	return nullptr;
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
