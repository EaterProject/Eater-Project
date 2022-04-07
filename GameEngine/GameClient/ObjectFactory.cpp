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
#include "ClientComponent.h"
#include "HealingDrone.h"
#include "AttackDrone.h"
#include "Bullet.h"
#include "Potal.h"
#include "ClientObjectManager.h"
#include "ManaStone.h"


ObjectFactory::ObjectFactory()
{
	PlayerObject		= nullptr;
	PlayerMainCamera	= nullptr;
	mOBJ_GM				= nullptr;
}

ObjectFactory::~ObjectFactory()
{
	PlayerObject = nullptr;
	PlayerMainCamera = nullptr;
	mOBJ_GM = nullptr;
}

void ObjectFactory::Initialize(ClientObjectManager* mGM)
{
	mOBJ_GM = mGM;
}

void ObjectFactory::Release()
{
	PlayerObject		= nullptr;
	PlayerMainCamera	= nullptr;
	mOBJ_GM				= nullptr;
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

Bullet* ObjectFactory::CreateBullet(float x, float y, float z)
{
	GameObject* DroneBullet = Instance("Bullet");
	MeshFilter* mMeshFilter = DroneBullet->AddComponent<MeshFilter>();
	Collider*	mCollider	= DroneBullet->AddComponent<Collider>();
	Bullet*		mBullet		= DroneBullet->AddComponent<Bullet>();

	mBullet->isLife = false;
	DroneBullet->GetTransform()->Position = { x,y,z };
	DroneBullet->SetTag(FindTagNumber("Bullet"));
	DroneBullet->Name = "Bullet";
	return mBullet;
}

MonsterA* ObjectFactory::CreateMonsterA(float x, float y, float z)
{
	GameObject* Object_Monster = Instance("MonsterA");
	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();
	MonsterA* monster = Object_Monster->AddComponent<MonsterA>();
	int num = FindTagNumber("Monster");
	Object_Monster->SetTag(num);
	Object_Monster->GetTransform()->Position = { x,y,z };
	monster->SetOBjManager(mOBJ_GM);
	return monster;
}

MonsterB* ObjectFactory::CreateMonsterB(float x, float y, float z)
{
	GameObject* Object_Monster = Instance("MonsterB");
	Object_Monster->AddComponent<MeshFilter>();
	Object_Monster->AddComponent<AnimationController>();
	Object_Monster->AddComponent<Collider>();
	Object_Monster->AddComponent<Rigidbody>();
	MonsterB* monster = Object_Monster->AddComponent<MonsterB>();
	Object_Monster->SetTag(FindTagNumber("Monster"));
	Object_Monster->GetTransform()->Position = { x,y,z };
	return monster;
}

ManaStone* ObjectFactory::CreateManaStone(float x, float y, float z)
{
	return nullptr;
}

HealingDrone* ObjectFactory::CreateHealingDrone(float x, float y, float z)
{
	GameObject* Healing		= Instance("HealingDrone");
	MeshFilter*		MF		= Healing->AddComponent<MeshFilter>();
	HealingDrone*   Drone	= Healing->AddComponent<HealingDrone>();
	
	Drone->SetPlayer(PlayerObject);
	return Drone;
}

AttackDrone* ObjectFactory::CreateAttackDrone(float x, float y, float z)
{
	GameObject*		Drone		 = Instance("AttackDrone");
	MeshFilter*		mMeshFileter = Drone->AddComponent<MeshFilter>();
	AttackDrone*	mAttackDrone = Drone->AddComponent<AttackDrone>();
	Collider*		mCollider	 = Drone->AddComponent<Collider>();

	mAttackDrone->SetOBjManager(mOBJ_GM);
	Drone->GetTransform()->Position = {x,y,z};

	return mAttackDrone;
}

Potal* ObjectFactory::CreatePortal(float x, float y, float z)
{
	GameObject* mPotalObj = Instance();
	MeshFilter* mMeshFileter = mPotalObj->AddComponent<MeshFilter>();
	Potal*		mPotal		 = mPotalObj->AddComponent<Potal>();

	mPotal->SetOBjManager(mOBJ_GM);
	mPotalObj->GetTransform()->Position = { x,y,z };
	return mPotal;
}


