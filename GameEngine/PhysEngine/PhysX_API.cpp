#include "PhysX_API.h"
#include "PhysEngine.h"
#include "Phys_Collider_Box.h"
#include "Phys_Collider_Capsule.h"
#include "Phys_Collider_Sphere.h"
#include "Phys_Collider_Triangle.h"


PhysEngine* Engine = nullptr;
void PhysX_Initialize(int ThreadCount, bool OnDebug)
{
	Engine = new PhysEngine();
	Engine->Initialize(ThreadCount,nullptr,OnDebug);
}

void PhysX_Release()
{
	if (Engine != nullptr)
	{
		Engine->Release();
	}
}

void PhysX_Update(float m_time)
{
	if (Engine != nullptr)
	{
		Engine->Update(m_time);
	}
}

void PhysX_Create_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Create_Actor(data);
	}
}

PhysData* PhysX_Create_Data()
{
	return Engine->Create_PhysData();
}

void PhysX_Update_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Update_Actor(data);
	}
}

void PhysX_Delete_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Delete_Actor(data);
	}
}

 bool PhysX_RayCast(PhysRayCast* ray)
{
	 return Engine->RayCast(ray);
}

 Phys_Collider_Triangle* CreateCollider_Triangle()
 {
	 Phys_Collider_Triangle* Triangle = new Phys_Collider_Triangle();
	 Triangle->Initialize();
	 return Triangle;
 }

