#include "NavigationManager.h"
#include "LoadManager.h"
#include "MainHeader.h"
#include "EngineData.h"
NavigationManager::NavigationManager()
{
	mColliderBuffer = nullptr;
}

NavigationManager::~NavigationManager()
{
	mColliderBuffer = nullptr;
}

void NavigationManager::Initialize()
{

}

void NavigationManager::Update()
{

}

void NavigationManager::SetNavMeshCollider(std::string&& ColliderName)
{
	mColliderBuffer = LoadManager::GetColliderBuffer(ColliderName);
}

void NavigationManager::SetNavMeshCollider(std::string& ColliderName)
{
	mColliderBuffer = LoadManager::GetColliderBuffer(ColliderName);
}

void NavigationManager::SettingNavMesh()
{
	if (mColliderBuffer == nullptr)
	{
		DebugPrint("NavMeshManager�� Collider�� �����ϴ�");
	}

	mColliderBuffer->VertexArray[0];
	mColliderBuffer->IndexArray[0];
	for (int i = 0; i < mColliderBuffer->IndexArrayCount; i++)
	{




	}
}
