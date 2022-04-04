#include "ObjectFactory.h"
#include "MainHeader.h"
#include "GameObject.h"

//엔진쪽 컨퍼넌트
#include "Camera.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Rigidbody.h"
#include "Transform.h"

//클라이언트쪽 컨퍼넌트
#include "Player.h"
#include "PlayerCamera.h"
#include "Collider.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "MonsterBase.h"
#include "HealingDrone.h"
#include "AttackDrone.h"


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
	//툴에서 만들어놓은 플레이어, 카메라를 가져옴
	PlayerObject		= FindGameObjectTag("Player");
	PlayerMainCamera	= FindGameObjectTag("MainCam");

	//클라이언트쪽 컨퍼넌트 Add
	PlayerObject->AddComponent<Player>();
	PlayerMainCamera->GetComponent<Camera>()->ChoiceMainCam();
	PlayerMainCamera->AddComponent<PlayerCamera>();

	return PlayerObject;
}

MonsterBase* ObjectFactory::CreateMonster(float x, float y, float z,MONSTER_TYPE Type)
{
	GameObject* Object_Monster = Instance();
 	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();
	Object_Monster->SetTag(6);
	MonsterBase* Base = Object_Monster->AddComponent<MonsterBase>();
	Base->SetPlayer(PlayerObject);
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

GameObject* ObjectFactory::CreateHealingDrone()
{
	GameObject* Healing		= Instance();
	MeshFilter*		MF		= Healing->AddComponent<MeshFilter>();
	HealingDrone*   Drone	= Healing->AddComponent<HealingDrone>();
	MF->SetModelName("drone");
	Drone->SetPlayer(PlayerObject);
	return Healing;
}

AttackDrone* ObjectFactory::CreateAttackDrone(float x, float y, float z)
{
	GameObject* Drone = Instance();
	MeshFilter*		MF		= Drone->AddComponent<MeshFilter>();
	AttackDrone*	DR		= Drone->AddComponent<AttackDrone>();
	Collider*		Col		= Drone->AddComponent<Collider>();
	MF->SetModelName("drone");
	Col->SetSphereCollider(10);
	Col->SetTrigger(true);
	Drone->GetTransform()->Position = {x,y,z};
	return DR;
}

GameObject* ObjectFactory::CreatePortal()
{
	return nullptr;
}
