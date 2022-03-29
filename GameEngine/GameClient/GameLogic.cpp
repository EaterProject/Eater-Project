#include "GameLogic.h"
#include "MainHeader.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"

GameLogic::GameLogic()
{
	Factory = nullptr;
}

GameLogic::~GameLogic()
{

}

void GameLogic::Initialize()
{
	//������Ʈ �Ŵ��� ����
	Factory = new ObjectFactory();
	Factory->Initialize();

	FindGameObjectTags("Potal", &PotalList);

	Factory->CreateMonster(0, 0, 0);
}

void GameLogic::Release()
{
	Factory->Release();
	delete Factory;
}

void GameLogic::Update()
{
	static float Count = 0;
	static float PosZ = 0;
	Count += GetDeltaTime();



	if (Count > 10)
	{
		float x, y, z;
		for (int i = 0; i < PotalList.size(); i++)
		{
			x = PotalList[i]->GetTransform()->Position.x;
			y = PotalList[i]->GetTransform()->Position.y;
			z = PotalList[i]->GetTransform()->Position.z;
			Factory->CreateMonster(x, y, z);
		}

		Count = 0;
		PosZ += 2;
	}
}
