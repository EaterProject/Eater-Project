#include "ClientObjectManager.h"

//엔진 컨퍼넌트
#include "Player.h"
#include "ObjectFactory.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

//클라이언트 컨퍼넌트
#include "PlayerCamera.h"
#include "ClientComponent.h"
#include "MonsterA.h"
#include "MonsterB.h"
#include "Potal.h"
#include "Drone.h"
#include "Bullet.h"
#include "FontImage.h"

FontImage* ClientObjectManager::mFontImage;
ClientObjectManager::ClientObjectManager()
{
	mFactory		= nullptr;
	PlayerObject	= nullptr;
	DroneList		= nullptr;
}

ClientObjectManager::~ClientObjectManager()
{
	mFactory		= nullptr;
	PlayerObject	= nullptr;
}

void ClientObjectManager::Initialize(ObjectFactory* Factory)
{
	//생성 펙토리 받기
	mFactory = Factory;

	//포탈 태그가 붙어있는 오브젝트를 모두 가져와 리스트에 담아놓는다
	FindGameObjectTags("ManaPoint", &ManaPoint_List);

	CreateObjectMemorySize();

	mFactory->CreatePlayer();

	mFontImage = mFactory->CreateFontImage(0, 0);

	
}

void ClientObjectManager::Release()
{
	mFactory = nullptr;
}

Bullet* ClientObjectManager::GetBullet()
{
	int Size = (int)Bullet_List.size();
	for (int i = 0; i < Size; i++)
	{
		if (Bullet_List[i]->GetLife() == false)
		{
			Bullet_List[i]->SetLife(true);
			return Bullet_List[i];
		}
	}

	//없으면 Null
	return nullptr;
}

Transform* ClientObjectManager::GetPlayerTransform()
{
	return PlayerObject->GetTransform();
}

void ClientObjectManager::SetCombo(int Number)
{
	mFontImage->SetCombo(Number);
}

void ClientObjectManager::CreateObjectMemorySize()
{
	int ManaCount = ManaPoint_List.size();
	for (int i = 0; i < ManaCount; i++)
	{
		Vector3 point  = ManaPoint_List[i]->GetTransform()->Position;
		mFactory->CreateManaStone(point.x, point.y, point.z);
	}
}

void ClientObjectManager::OnActivePotal(bool isActive, int index)
{
	int Size = (int)Potal_List.size();
	if (index == -1) 
	{
		//모든 포탈 활성화
		for (int i = 0; i < Size; i++)
		{
			Potal_List[i]->OnPotalActivation(isActive);
		}
	}
	else
	{
		if (Size - 1 >= index)
		{
			Potal_List[index]->OnPotalActivation(isActive);
		}
	}
}




