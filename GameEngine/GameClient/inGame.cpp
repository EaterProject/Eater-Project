#include "EaterEngineAPI.h"
#include "EngineData.h"
#include "inGame.h"
#include "GameLogic.h"
#include "Material.h"
#include "Terrain.h"
#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"
#include "Camera.h"
#include "EngineData.h"

void InGame::Awake()
{
	//게임 로직관리 매니저를 생성
	Logic = new GameLogic();

	//Tool에서 만들어놓은 씬을 로드
	Load("../Assets/Scene/InGame.Scene");

	// 오클루더 설정..
	AddOccluder("Dome_program_0");
		
	//BakeSkyLightMap("SkyLight_0", false);
	//BakeSkyLightMap("Dome_HDR_5", false);
	//SetSkyLight("SkyLight_0", 0);
	//SetSkyLight("Dome_HDR_5", 1);
	//
	SetColorGradingBaseTexture("Dome_LUT_4");
	/// Blend Factor
	SetColorGradingBlendFactor(0.0f);
	//로직매니저 초기화
	Logic->Initialize();

	GameObject* testobj = Instance_Terrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);

	Option = GetRenderOptionData();
}

void InGame::Update()
{
	if (GetKeyDown(VK_F1)) 
	{
		Option->DebugOption ^= DEBUG_OPTION::DEBUG_MODE;
		RenderSetting();
	}

	Logic->Update();
}

void InGame::End()
{

}
