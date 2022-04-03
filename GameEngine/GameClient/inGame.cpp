#include "MainHeader.h"
#include "inGame.h"
#include "GameLogic.h"
void InGame::Awake()
{
	//���� �������� �Ŵ����� ����
	Logic = new GameLogic();
	//Tool���� �������� ���� �ε�
	Load("../Assets/Scene/intro.Scene");
	//�����Ŵ��� �ʱ�ȭ
	Logic->Initialize();
}

void InGame::Start()
{

}

void InGame::Update()
{
	Logic->Update();
}

void InGame::End()
{
	Logic->Release();
}
