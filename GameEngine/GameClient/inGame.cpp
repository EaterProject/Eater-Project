#include "EaterEngineAPI.h"
#include "inGame.h"
#include "GameLogic.h"
#include "Material.h"
#include "Terrain.h"
#include "GameObject.h"

void InGame::Awake()
{
	//���� �������� �Ŵ����� ����
	Logic = new GameLogic();

	//Tool���� �������� ���� �ε�
	Load("../Assets/Scene/inGame.Scene");

	BakeSkyLightMap("HDRI_2_CubeMap", false);

	//SetSkyLight("Day");
	SetSkyLight("HDRI_2_CubeMap");

	//SetEnvironment("Day");
	SetSkyCube("HDRI_2_CubeMap");

	//�����Ŵ��� �ʱ�ȭ
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
