#pragma
#include "MainHeader.h"
#include "GameManager.h"
#include "intro.h"
#include "inGame.h"
#include "Loading.h"
#include "TestScene.h"



GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

void GameManager::Start(HWND _g_hWnd)
{
	//엔진 초기화 (매니저들 생성)
	EngineInitialize(_g_hWnd,true);
	
	CreateScene<intro>("intro");
	CreateScene<InGame>("inGame");
	CreateScene<TestScene>("TestScene");
	CreateScene<Loading>("Loading");
	EditorSetting();
	//ChoiceScene("TestScene");
	ChoiceScene("Loading");
	//ChoiceScene("intro");
	//ChoiceScene("inGame");
}

void GameManager::Update()
{
	//넘겨준 스크린 바탕으로 스크린의 업데이트를 돌려줌
	UpdateEngine();
}