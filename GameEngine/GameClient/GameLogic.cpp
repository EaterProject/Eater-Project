#include "GameLogic.h"
#include "ObjectFactory.h"
#include "GameObject.h"

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
}

void GameLogic::Release()
{
	Factory->Release();
	delete Factory;
}
