#include "EaterEngineAPI.h"
#include "inGame.h"
#include "GameLogic.h"
#include "Material.h"
#include "Terrain.h"
#include "GameObject.h"

void InGame::Awake()
{
	//게임 로직관리 매니저를 생성
	Logic = new GameLogic();

	//Tool에서 만들어놓은 씬을 로드
	Load("../Assets/Scene/inGame.Scene");

	BakeSkyLightMap("HDRI_2_CubeMap", false);

	//SetSkyLight("Day");
	SetSkyLight("HDRI_2_CubeMap");

	//SetEnvironment("Day");
	SetSkyCube("HDRI_2_CubeMap");

	//로직매니저 초기화
	Logic->Initialize();

	GameObject* testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);
}

void InGame::Update()
{
	Logic->Update();
}

void InGame::End()
{
	Logic->Release();
}
