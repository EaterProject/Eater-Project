#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"

//클라이언트쪽 컨퍼넌트
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
	//툴에서 만들어놓은 플레이어, 카메라를 가져옴
	PlayerObject		= FindGameObjectTag("Player");
	PlayerMainCamera	= FindGameObjectTag("MainCam");


	//클라이언트쪽 컨퍼넌트 Add
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
