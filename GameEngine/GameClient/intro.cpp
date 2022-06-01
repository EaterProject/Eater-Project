#include "EaterEngineAPI.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Rigidbody.h"
#include "ClientNetworkManager.h"
#include "PhysData.h"
#include "PortIPDefine.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "MonsterA.h"
#include "ParticleSystem.h"
#include "Collider.h"

void intro::Awake()
{
	Load("../Assets/Scene/intro.Scene");

	GameObject* MainCam			= FindGameObjectTag("MainCam");
	GameObject* PlayerObject	= FindGameObjectTag("Player");
	PlayerObject->AddComponent<Player>();


	Camera*	Cam =  MainCam->GetComponent<Camera>();
	MainCam->AddComponent<PlayerCamera>();
	Cam->ChoiceMainCam();

	BakeSkyLightMap("Night", false);
	SetSkyLight("Night");

	SetSkyCube("Night");
}

void intro::Update()
{
	
}

void intro::End()
{

}

void intro::CreateTerrain()
{
	///ÅÍ·¹ÀÎ
	testobj = Instance_Terrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetMeshName("Terrain");
	//mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(31.0f);
}


