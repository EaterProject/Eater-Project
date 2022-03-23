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
	//���� �ʱ�ȭ (�Ŵ����� ����)
	EngineInitialize(_g_hWnd,true);
	
	CreateScene<intro>("intro");
	CreateScene<InGame>("inGame");
	CreateScene<TestScene>("TestScene");
	CreateScene<Loading>("Loading");

	ChoiceScene("Loading");
	//ChoiceScene("TestScene");
	//ChoiceScene("intro");
	//ChoiceScene("inGame");
}

void GameManager::Update()
{
	//�Ѱ��� ��ũ�� �������� ��ũ���� ������Ʈ�� ������
	UpdateEngine();
}