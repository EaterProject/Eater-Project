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

	//���� ���� �浹�� ��ü�� �浹����
	bool RayCast(PhysRayCast* ray);

	//�浹�� ��� ��ü�� ����
	bool RayCastMultiple(PhysRayCast* ray);
private:
	physx::PxScene* m_Scene;
};