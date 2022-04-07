#include "MainHeader.h"
#include "inGame.h"
#include "GameLogic.h"
void InGame::Awake()
{
	//게임 로직관리 매니저를 생성
	Logic = new GameLogic();

	//Tool에서 만들어놓은 씬을 로드
	Load("../Assets/Scene/inGame.Scene");
	//로직매니저 초기화
	AddOccluder("Dome_Occluder_0");

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
