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
#include "UICanvas.h"
#include "GateDoor.h"
#include "Boss.h"
#include "CameraManager.h"



ObjectFactory::ObjectFactory()
{

}

ObjectFactory::~ObjectFactory()
{

}

void ObjectFactory::Release()
{

}

GameObject* ObjectFactory::CreatePlayer()
{
	//플레이어와 카메라 오브젝트를 생성
	GameObject* PlayerObject		= Instance();
	
	GameObject* PlayerCollider		= Instance();
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
	
	DroneObject->AddComponent<MeshFilter>();
	DroneObject->AddComponent<Drone>();

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
	for (int i = 0; i < 1; i++)
	{
		Vector3 point = ManaPoint_List[i]->GetTransform()->Position;
		

		GameObject* Object_ManaStone = Instance("ManaStone");
		Object_ManaStone->AddComponent<MeshFilter>();
		ManaStone* mMana = Object_ManaStone->AddComponent<ManaStone>();
		Object_ManaStone->GetTransform()->Position = point;

		switch (i)
		{
		case 0:
			mMana->SetMonsterCount(5, 0);
			break;
		case 1:
			mMana->SetMonsterCount(4, 1);
			break;
		case 2:
			mMana->SetMonsterCount(3, 2);
			break;
		}
	}
	return nullptr;
}

GameObject* ObjectFactory::CreateFontImage()
{
	GameObject* Object_FontImage = Instance();
	Object_FontImage->AddComponent<ComboFont>();
	return Object_FontImage;
}

GameObject* ObjectFactory::CreateUICanvas()
{
	GameObject* Object_PlayerState = Instance();
	Object_PlayerState->AddComponent<UICanvas>();
	return Object_PlayerState;
}

GameObject* ObjectFactory::CreateBoss()
{
	GameObject* Object_Boss = Instance();
	Object_Boss->AddComponent<Boss>();
	Object_Boss->AddComponent<MeshFilter>();
	Object_Boss->AddComponent<AnimationController>();
	Object_Boss->AddComponent<Collider>();
	Object_Boss->AddComponent<Rigidbody>();
	return Object_Boss;
}

GameObject* ObjectFactory::CreateGate_In()
{
	GameObject* Object = Instance();
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	mMeshFilter->SetModelName("Dome_GateDoor_IN");
	return Object;
}

GameObject* ObjectFactory::CreateGate_Out()
{
	GameObject* Object = Instance();
	MeshFilter* mMeshFilter = Object->AddComponent<MeshFilter>();
	mMeshFilter->SetModelName("Dome_GateDoor_OUT");
	return Object;
}

GameObject* ObjectFactory::CreateGate_Manager()
{
	GameObject* Object = Instance();
	Object->AddComponent<GateDoor>();
	return Object;
}

GameObject* ObjectFactory::CreateCameraManager()
{
	GameObject* Object = Instance_Camera();
	Object->AddComponent<CameraManager>();
	return Object;
}

GameObject* ObjectFactory::CreateBossWeapon()
{
	GameObject* Object = Instance();
	Object->AddComponent<MeshFilter>();
	return Object;
}





