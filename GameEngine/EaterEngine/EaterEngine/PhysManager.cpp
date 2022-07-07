#include "PhysManager.h"
#include "PhysX_API.h"
#include "PhysData.h"

PhysManager::PhysManager()
{
	Phys = nullptr;
	Frame = 1.0f / 60.0f;
}

PhysManager::~PhysManager()
{

}

void PhysManager::Initialize()
{
	PhysX_Initialize(1,true);
}

void PhysManager::Release()
{
	PhysX_Release();
}

void PhysManager::Update(float m_Time)
{
	PhysX_Update(m_Time);
}

bool PhysManager::RayCast(PhysRayCast* ray)
{
	return PhysX_RayCast(ray);
}

