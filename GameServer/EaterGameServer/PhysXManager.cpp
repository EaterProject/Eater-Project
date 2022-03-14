#include "PhysXManager.h"
#include "PhysX_API.h"

PhysXManager::PhysXManager()
{

}

PhysXManager::~PhysXManager()
{

}

void PhysXManager::Initialize()
{
#ifdef _DEBUG
	PhysX_Initialize(4, nullptr, true);
#else
	PhysX_Initialize(4, nullptr, true);
#endif
}

void PhysXManager::Release()
{
	PhysX_Release();
}

void PhysXManager::Update(float _dTime)
{
	PhysX_Update(_dTime);
}

void PhysXManager::Create_Actor(PhysData* _Data)
{
	PhysX_Create_Actor(_Data);
}

void PhysXManager::Update_Actor(PhysData* _Data)
{
	PhysX_Update_Actor(_Data);
}

void PhysXManager::Delete_Actor(PhysData* _Data)
{
	PhysX_Delete_Actor(_Data);
}

void PhysXManager::RayCast(PhysRayCast* _Ray)
{
	PhysX_RayCast(_Ray);
}
