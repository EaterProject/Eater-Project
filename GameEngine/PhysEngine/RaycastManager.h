#pragma once


namespace physx
{
	class PxScene;
	class PxVec3;
}
using namespace physx;

class PhysRayCast;

class RaycastManager
{
public:
	RaycastManager();
	~RaycastManager();

	void Initialize(physx::PxScene* mScene);

	//가장 먼저 충돌한 객체의 충돌여부
	bool RayCast(PhysRayCast* ray);

	//충돌한 모든 객체의 여부
	bool RayCastMultiple(PhysRayCast* ray);
private:
	physx::PxScene* m_Scene;
};