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
	//오브젝트 매니저 생성
	Factory = new ObjectFactory();
	Factory->Initialize();

	FindGameObjectTags("Potal", &PotalList);
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



	if (Count > 1)
	{
		float x, y, z;
		//x = PotalList[i]->GetTransform()->Position.x;
		//y = PotalList[i]->GetTransform()->Position.y;
		//z = PotalList[i]->GetTransform()->Position.z;

		Factory->CreateMonster(0, 0, PosZ);
		Count = 0;
		PosZ += 2;
	}
}
