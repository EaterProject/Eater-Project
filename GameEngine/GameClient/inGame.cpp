#include "EaterEngineAPI.h"
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

	RenderOption* option = GetRenderOptionData();
	option->RenderingOption ^= RENDER_OPTION::RENDER_FOG;

	RenderSetting();


	//Tool에서 만들어놓은 씬을 로드
	Load("../Assets/Scene/inGame.Scene");

	// 오클루더 설정..
	//AddOccluder("Dome_program_0");

	BakeSkyLightMap("SkyLight_HDRI", false);

	//SetSkyLight("Day");
	SetSkyLight("SkyLight_HDRI");

	//SetEnvironment("Day");
	SetSkyCube("SkyCube_HDRI");

	//로직매니저 초기화
	Logic->Initialize();

	Sound_Play_BGM("InGame_InDoor");

	//GameObject* testobj = InstanceTerrain("Terrain");
	//Terrain* mTerrain = testobj->GetComponent<Terrain>();
	//mTerrain->SetLayerName("terrain_ground_A_BaseColor", "terrain_ground_A_Normal", "terrain_ground_A_ORM");
	//mTerrain->SetLayerName("terrain_ground_B_BaseColor", "terrain_ground_B_Normal", "terrain_ground_B_ORM");
	//mTerrain->SetLayerName("terrain_ground_C_BaseColor", "terrain_ground_C_Normal", "terrain_ground_C_ORM");
	//mTerrain->SetLayerName("terrain_ground_D_BaseColor", "terrain_ground_D_Normal", "terrain_ground_D_ORM");
	//mTerrain->SetMeshName("Terrain");
	//mTerrain->SetTextureTiling(31.0f);



	//GameObject* ui_object = nullptr;
	//Image* ui_image = nullptr;
	//RectTransform* ui_rectTR = nullptr;
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_minimap");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_RIGHT_TOP);
	//ui_rectTR->AddPosition(25.0f, 75.0f);
	//ui_rectTR->SetRotation(0.0f, 0.0f, -90.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_combo");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_RIGHT);
	//ui_rectTR->AddPosition(-25.0f, 0.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_playercounter_1");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_MIDDLE_CENTER);
	//ui_rectTR->AddPosition(-75.0f, -25.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp_back");
	//
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(50.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_player_hp");
	//ui_image->SetImageColor(255, 0, 0);
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(50.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_switch");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(50.0f, -150.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_skill_rb");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(400.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_skill_rb");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(400.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_skill_space");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(475.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);
	//
	//ui_object = InstanceUI("UI");
	//ui_image = ui_object->AddComponent<Image>();
	//ui_image->SetTexture("ingame_skill_e");
	//ui_rectTR = ui_object->GetComponent<RectTransform>();
	//ui_rectTR->SetImagePivot(RECT_PIVOT::PIVOT_LEFT_BOTTOM);
	//ui_rectTR->AddPosition(550.0f, -50.0f);
	//ui_rectTR->SetScale(0.5f, 0.5f);


	DebugCam = GetDebugCamera();
	MainCame = GetMainCamera();
}

void InGame::Update()
{
	if (GetKeyDown(VK_F10))
	{
		DebugCam->GetComponent<Camera>()->ChoiceMainCam();
	}
	if(GetKeyDown(VK_F11))
	{
		MainCame->GetComponent<Camera>()->ChoiceMainCam();
	}



	Logic->Update();
}

void InGame::End()
{
	Logic->Release();
}
