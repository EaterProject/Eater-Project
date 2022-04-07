#include "MainHeader.h"
#include "inGame.h"
#include "GameLogic.h"
void InGame::Awake()
{
	//���� �������� �Ŵ����� ����
	Logic = new GameLogic();

	//Tool���� �������� ���� �ε�
	Load("../Assets/Scene/inGame.Scene");
	//�����Ŵ��� �ʱ�ȭ
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
