#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
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
#include "ClientComponent.h"
#include "Drone.h"
#include "Bullet.h"
#include "MessageManager.h"
#include "ManaStone.h"
#include "FontImage.h"
#include "ComboFont.h"
#include "PlayerStateImage.h"



ObjectFactory::ObjectFactory()
{
	PlayerObject		= nullptr;
	PlayerMainCamera	= nullptr;
}

ObjectFactory::~ObjectFactory()
{
	PlayerObject = nullptr;
	PlayerMainCamera = nullptr;
}

void ObjectFactory::Release()
{
	PlayerObject		= nullptr;
	PlayerMainCamera	= nullptr;
}

GameObject* ObjectFactory::CreatePlayer()
{
	//플레이어와 카메라 오브젝트를 생성
	GameObject* PlayerObject		= Instance();
	GameObject* PlayerMainCamera	= Instance_Camera("Camera");
	GameObject* PlayerCollider		= Instance();
	GameObject* PlayerWeapon		= Instance();
	GameObject* DroneObject			= Instance();
	GameObject* PlayerPoint			= FindGameObjectTag("PlayerPoint");
	
	//플레이어 생성
	PlayerObject->SetTag("Player");
	PlayerObject->AddComponent<Player>();
	PlayerObject->AddComponent<MeshFilter>();
	PlayerObject->AddComponent<AnimationController>();
	PlayerObject->GetTransform()->Position = PlayerPoint->GetTransform()->Position;

	//콜라이더 객체 생성
	PlayerCollider->SetTag("PlayerCollider");
	PlayerCollider->AddComponent<Collider>();
	
	//플레이어 무기 생성
	PlayerWeapon->SetTag("Weapon");
	PlayerWeapon->AddComponent<MeshFilter>();

	DroneObject->AddComponent<MeshFilter>();
	DroneObject->AddComponent<Drone>();

	//카메라 생성
	Camera* Main = PlayerMainCamera->GetComponent<Camera>();
	Main->ChoiceMainCam();
	PlayerMainCamera->AddComponent<PlayerCamera>();

	return PlayerObject;
}

GameObject* ObjectFactory::CreateBullet()
{
	GameObject* DroneBullet = Instance("Bullet");
	MeshFilter* mMeshFilter = DroneBullet->AddComponent<MeshFilter>();
	Collider*	mCollider	= DroneBullet->AddComponent<Collider>();
	Bullet*		mBullet		= DroneBullet->AddComponent<Bullet>();

	mBullet->isLife = false;
	DroneBullet->SetTag(FindTagNumber("Bullet"));
	DroneBullet->Name = "Bullet";
	return DroneBullet;
}

GameObject* ObjectFactory::CreateMonsterA()
{
	GameObject* Object_Monster = Instance("MonsterA");
	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();

	MonsterA* monster = Object_Monster->AddComponent<MonsterA>();

	//MonsterA_List.push_back(monster);
	return Object_Monster;
}

GameObject* ObjectFactory::CreateMonsterB()
{
	GameObject* Object_Monster = Instance("MonsterB");
	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();

	MonsterB* monster = Object_Monster->AddComponent<MonsterB>();
	return Object_Monster;
}

GameObject* ObjectFactory::CreateManaStone()
{
	int ManaCount = ManaPoint_List.size();
	for (int i = 0; i < ManaCount; i++)
	{
		Vector3 point = ManaPoint_List[i]->GetTransform()->Position;
		

		GameObject* Object_ManaStone = Instance("ManaStone");
		Object_ManaStone->AddComponent<MeshFilter>();
		ManaStone* mMana = Object_ManaStone->AddComponent<ManaStone>();
		Object_ManaStone->GetTransform()->Position = point;
	}
	return nullptr;
}

GameObject* ObjectFactory::CreateFontImage()
{
	GameObject* Object_FontImage = Instance();
	Object_FontImage->AddComponent<ComboFont>();
	return Object_FontImage;
}

GameObject* ObjectFactory::CreatePlayerState()
{
	GameObject* Object_PlayerState = Instance();
	Object_PlayerState->AddComponent<PlayerStateImage>();
	return Object_PlayerState;
}





