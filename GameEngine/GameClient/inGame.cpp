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
	//���� �������� �Ŵ����� ����
	Logic = new GameLogic();
	
	RenderOption* option = GetRenderOptionData();
	option->RenderingOption ^= RENDER_OPTION::RENDER_FOG;
	
	RenderSetting();
	
	
	//Tool���� �������� ���� �ε�
	Load("../Assets/Scene/inGame.Scene");
	
	// ��Ŭ��� ����..
	//AddOccluder("Dome_program_0");
	
	BakeSkyLightMap("SkyLight_HDRI", false);
	
	BakeSkyLightMap("SkyLight_0", false);
	BakeSkyLightMap("SkyLight_1", false);
	
	SetSkyLight("SkyLight_0", 0);
	SetSkyLight("SkyLight_1", 1);
	
	SetSkyCube("SkyCube");
	
	//�����Ŵ��� �ʱ�ȭ
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
