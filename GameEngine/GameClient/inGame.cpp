#include "EaterEngineAPI.h"
#include "inGame.h"
#include "GameLogic.h"

void InGame::Awake()
{
	//���� �������� �Ŵ����� ����
	Logic = new GameLogic();

	//Tool���� �������� ���� �ε�
	Load("../Assets/Scene/inGame.Scene");

	BakeSkyLightMap("HDRI_2_CubeMap", false);

	//SetSkyLight("Day");
	SetSkyLight("HDRI_2_CubeMap");

	//SetEnvironment("Day");
	SetSkyCube("HDRI_2_CubeMap");

	//�����Ŵ��� �ʱ�ȭ
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
