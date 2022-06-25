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
#include "MessageManager.h"
#include "ManaStone.h"
#include "FontImage.h"
#include "ComboFont.h"
#include "UICanvas.h"
#include "GateDoor.h"
#include "Boss.h"
#include "CameraManager.h"
#include "BossWeapon.h"
#include "BossFriend.h"
#include "UIEffect.h"
#include "UITitle.h"



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

	Vector3 position = PlayerPoint->GetTransform()->GetPosition();
	PlayerObject->GetTransform()->SetPosition(position);

	//콜라이더 객체 생성
	PlayerCollider->SetTag("PlayerCollider");
	PlayerCollider->AddComponent<Collider>();
	
	DroneObject->AddComponent<MeshFilter>();
	DroneObject->AddComponent<Drone>();

	return PlayerObject;
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
	for (int i = 0; i < ManaCount; i++)
	{
		Vector3 point = ManaPoint_List[i]->GetTransform()->GetPosition();
		

		GameObject* Object_ManaStone = Instance("ManaStone");
		Object_ManaStone->AddComponent<MeshFilter>();
		ManaStone* mMana = Object_ManaStone->AddComponent<ManaStone>();
		Object_ManaStone->GetTransform()->SetPosition(point);

		switch (i)
		{
		case 0:
			mMana->SetMonsterCount(3, 0);
			break;
		case 1:
			mMana->SetMonsterCount(3, 1);
			break;
		case 2:
			mMana->SetMonsterCount(2, 2);
			break;
		case 3:
			mMana->SetMonsterCount(3, 2);
			break;
		case 4:
			mMana->SetMonsterCount(2, 3);
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

GameObject* ObjectFactory::CreateUIEffect()
{
	GameObject* Object_Effect = Instance();
	Object_Effect->AddComponent<UIEffect>();
	return Object_Effect;
}

GameObject* ObjectFactory::CreateUITitle()
{
	GameObject* Object_Title = Instance();
	Object_Title->AddComponent<UITitle>();
	return Object_Title;
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
	Object->AddComponent<BossWeapon>();
	return Object;
}

GameObject* ObjectFactory::CreateBossFriend()
{
	GameObject* Object = Instance();
	Object->AddComponent<MeshFilter>();
	Object->AddComponent<BossFriend>();
	Object->AddComponent<AnimationController>();
	return Object;
}





