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
	//������Ʈ �Ŵ��� ����
	Factory = new ObjectFactory();
	Factory->Initialize();
}

void GameLogic::Release()
{
	Factory->Release();
	delete Factory;
}
