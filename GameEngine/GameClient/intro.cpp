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
#include "TestPlayer.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Collider.h"
//#include "Manastone.h"

void intro::Awake()
{
	LoadEnvironment("../Assets/Texture/Base/Night.dds");

	GameObject* gameobject = GetMainCamera();
	DebugCam = gameobject->GetComponent<Camera>();
	////
	////게임에 사용할 카메라를 만들어준다
	GameObject* Cam = InstanceCamera();
	MainCam = Cam->GetComponent<Camera>();
	PlayerCamera* PC = Cam->AddComponent<PlayerCamera>();
	MainCam->ChoiceMainCam();
	Cam->SetDontDestroy(true);
	//
	GameObject* Obj = Instance();
	Obj->AddComponent<MeshFilter>();
	Obj->AddComponent<TestPlayer>();
	Obj->AddComponent<AnimationController>();
	//Obj->AddComponent<Rigidbody>();
	//Obj->AddComponent<Collider>();
	Obj->SetDontDestroy(true);
	PC->Userobject = Obj;

	//StartNetwork();

	//LoadUnityFile("../Resources/Data/Collider.Eater");

	
	//Load("../Assets/Scene/intro.Scene");
	//CreateMap();
	SetEnvironment(true);
	//CreateTerrain();
}

void intro::Update()
{
	//ChangeCubeMap();
	//
	////플레이어 카메라
	//if (GetKeyDown(VK_F7))
	//{
	//	MainCam->ChoiceMainCam();
	//}
	//
	////자유시점 카메라
	//if (GetKeyDown(VK_F8))
	//{
	//	DebugCam->ChoiceMainCam();
	//}
}

void intro::End()
{

}

void intro::CreateTerrain()
{
	///터레인
	testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);
}

void intro::ChangeCubeMap()
{
	if (GetKeyUp('1'))
	{
		LoadEnvironment("../Resources/Texture/Environment/Day.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('2'))
	{
		LoadEnvironment("../Resources/Texture/Environment/Night.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('3'))
	{
		LoadEnvironment("../Resources/Texture/Environment/skybox1.dds");
		SetEnvironment(true);
	}
	if (GetKeyUp('4'))
	{
		LoadEnvironment("../Resources/Texture/Environment/TestSky.dds");
		SetEnvironment(true);
	}
}
