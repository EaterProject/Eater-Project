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
	
	BakeSkyLightMap("SkyLight_0", false);
	BakeSkyLightMap("SkyLight_1", false);
	
	SetSkyLight("SkyLight_0", 0);
	SetSkyLight("SkyLight_1", 1);
	
	SetSkyCube("SkyCube");
	
	//로직매니저 초기화
	Logic->Initialize();

	DebugCam = GetDebugCamera();
	MainCame = GetMainCamera();
}

void InGame::Update()
{
	//if (GetKeyDown(VK_F10))
	//{
	//	DebugCam->GetComponent<Camera>()->ChoiceMainCam();
	//}
	//if(GetKeyDown(VK_F11))
	//{
	//	MainCame->GetComponent<Camera>()->ChoiceMainCam();
	//}
	//
	//
	//
}

void InGame::End()
{
	//Logic->Release();
}
