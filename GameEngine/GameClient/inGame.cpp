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
#include "MessageManager.h"
#include "ClientTypeOption.h"
#include "MeshFilterSetting.h"

void InGame::Awake()
{
	//게임 로직관리 매니저를 생성
	Logic = new GameLogic();

	//Tool에서 만들어놓은 씬을 로드
	Load("../Assets/Scene/InGame.Scene");

	// 오클루더 설정..
	AddOccluder("Dome_program_0");

	// Color Grading
	SetColorGradingBaseTexture("LUT_outside");
	SetColorGradingBlendTexture("Dome_LUT_4");
	SetColorGradingBlendFactor(0.0f);

	//로직매니저 초기화
	Logic->Initialize();

	MeshFilterSetting::Initialize();

	GameObject* testobj = Instance_Terrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetTextureTiling(31.0f);

	Option = GetRenderOptionData();
	Option->RenderingOption = RENDER_SHADOW | RENDER_SSAO | RENDER_IBL | RENDER_FOG;
	Option->PostProcessOption = POSTPROCESS_BLOOM | POSTPROCESS_HDR | POSTPROCESS_COLORGRADING | POSTPROCESS_FXAA;
	RenderSetting();
}

void InGame::Update()
{
	if (GetKeyDown(VK_F1)) 
	{
		Option->DebugOption ^= DEBUG_OPTION::DEBUG_MODE;
		RenderSetting();
	}

	//if (GetKeyDown('1'))
	//{
	//	//MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_BOSS_);
	//}
	//
	//if (GetKeyDown('2'))
	//{
	//	bool test;
	//	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_GAME_END, &test);
	//	MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_CREDIT);
	//}
	//if (GetKeyDown('3'))
	//{
	//	MessageManager::GetGM()->SEND_Message(TARGET_CAMERA_MANAGER, MESSAGE_CAMERA_CINEMATIC_BOSS_START);
	//}

	Logic->Update();
}

void InGame::End()
{

}
