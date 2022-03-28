#include "MainHeader.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Rigidbody.h"
#include "ClientNetworkManager.h"
#include "PhysData.h"
#include "PortIPDefine.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Collider.h"

void intro::Awake()
{
	LoadEnvironment("../Assets/Texture/Base/Night.dds");

	GameObject* gameobject = GetMainCamera();
	DebugCam = gameobject->GetComponent<Camera>();
	//
	////���ӿ� ����� ī�޶� ������ش�
	GameObject* Cam = InstanceCamera();
	MainCam = Cam->GetComponent<Camera>();
	PlayerCamera* PC = Cam->AddComponent<PlayerCamera>();
	MainCam->ChoiceMainCam();
	Cam->SetDontDestroy(true);
	////
	GameObject* Obj = Instance();
	Obj->AddComponent<MeshFilter>();
	Obj->AddComponent<Player>();
	Obj->AddComponent<AnimationController>();
	//Obj->AddComponent<Rigidbody>();
	//Obj->AddComponent<Collider>();
	Obj->SetDontDestroy(true);
	PC->Userobject = Obj;
	
	Load("../Assets/Scene/intro.Scene");
	SetEnvironment(true);
}

void intro::Update()
{
	
}

void intro::End()
{

}

void intro::CreateTerrain()
{
	///�ͷ���
	testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);
}


