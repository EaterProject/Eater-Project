#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"

//������ ���۳�Ʈ
#include "Camera.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Rigidbody.h"
#include "Transform.h"

//Ŭ���̾�Ʈ�� ���۳�Ʈ
#include "Player.h"
#include "PlayerCamera.h"
#include "Collider.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "ClientComponent.h"
#include "Drone.h"
#include "Bullet.h"
#include "Potal.h"
#include "ClientObjectManager.h"
#include "ManaStone.h"
#include "FontImage.h"



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
	//�÷��̾�� ī�޶� ������Ʈ�� ����
	GameObject* PlayerObject		= Instance();
	GameObject* PlayerMainCamera	= Instance_Camera("Camera");
	GameObject* PlayerCollider		= Instance();
	GameObject* PlayerWeapon		= Instance();
	GameObject* DroneObject			= Instance();
	GameObject* PlayerPoint			= FindGameObjectTag("PlayerPoint");
	
	//�÷��̾� ����
	PlayerObject->SetTag("Player");
	PlayerObject->AddComponent<Player>();
	PlayerObject->AddComponent<MeshFilter>();
	PlayerObject->AddComponent<AnimationController>();
	PlayerObject->GetTransform()->Position = PlayerPoint->GetTransform()->Position;

	//�ݶ��̴� ��ü ����
	PlayerCollider->SetTag("PlayerCollider");
	PlayerCollider->AddComponent<Collider>();
	
	//�÷��̾� ���� ����
	PlayerWeapon->SetTag("Weapon");
	PlayerWeapon->AddComponent<MeshFilter>();

	DroneObject->AddComponent<MeshFilter>();
	DroneObject->AddComponent<Drone>();

	//ī�޶� ����
	Camera* Main = PlayerMainCamera->GetComponent<Camera>();
	Main->ChoiceMainCam();
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
	Object_Monster->GetTransform()->Position = { x,y,z };
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
	Object_Monster->GetTransform()->Position = { x,y,z };
	return monster;
}

ManaStone* ObjectFactory::CreateManaStone(float x, float y, float z)
{
	GameObject* Object_ManaStone = Instance("ManaStone");
	Object_ManaStone->AddComponent<MeshFilter>();
	ManaStone* mMana = Object_ManaStone->AddComponent<ManaStone>();
	Object_ManaStone->GetTransform()->Position = { x,y,z };
	return mMana;
}

FontImage* ObjectFactory::CreateFontImage(float x, float y)
{
	GameObject* Object_FontImage = Instance();
	return Object_FontImage->AddComponent<FontImage>();
}





