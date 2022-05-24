#include "GameLogic.h"
#include "EaterEngineAPI.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"
#include "ClientObjectManager.h"
#include "ClientComponent.h"
#include "Collider.h"
#include "Camera.h"

GameLogic::GameLogic()
{
	FactoryGM	= nullptr;
	ObjectGM	= nullptr;
}

GameLogic::~GameLogic()
{
	delete FactoryGM;
	delete ObjectGM;
}

void GameLogic::Initialize()
{
	//�Ŵ��� ����
	FactoryGM	= new ObjectFactory();
	ObjectGM	= new ClientObjectManager();

	//������Ʈ ���� �Ŵ���
	FactoryGM->Initialize(ObjectGM);
	ObjectGM->Initialize(FactoryGM);

	AddOccluder("Dome_program_0");
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{
	

}

