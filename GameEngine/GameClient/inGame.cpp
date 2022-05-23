#include "EaterEngineAPI.h"
#include "inGame.h"
#include "GameLogic.h"

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
}

void InGame::Update()
{
	Logic->Update();
}

void InGame::End()
{
	Logic->Release();
}
