#pragma once

#include "PxPhysicsAPI.h"
namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxSphereGeometry;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxTransform;
	struct PxCookingParams;
}

class PhysData;
class PhysMaterial;
class PhysCollider;

struct TriangleMeshData;

class Factory
{
public:
	Factory();
	~Factory();

	void Initialize(physx::PxPhysics* Phys, physx::PxScene* Scene, physx::PxCooking* Cooking);
	void CreateActoer(PhysData* data);
private:
	///Shape 셋팅
	physx::PxShapeFlags CreateShapeFlag(bool isTrigger);
	physx::PxShape*		CreateShape(PhysMaterial* m,PhysCollider* c);
private:
	///콜라이더 생성
	physx::PxShape*		CreateBoxCollider(physx::PxMaterial* m, PhysCollider* c);
	physx::PxShape*		CreateSphereCollider(physx::PxMaterial* m, PhysCollider* c);
	physx::PxShape*		CreateCapsuleCollider(physx::PxMaterial* m, PhysCollider* c);
	physx::PxShape*		CreateTriangleCollider(physx::PxMaterial* m , PhysCollider* c);
	physx::PxShape*		CreateHeightFieldCollider(physx::PxMaterial* m , PhysCollider* c);
private:
	///재질 생성
	physx::PxMaterial*	CreateMaterial(PhysMaterial* m);
private:
	///Actor 셋팅
	void CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
	void CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
private:
	void CreateTriangleBuffer(PhysData* data, physx::PxVec3* mVertex, physx::PxU32* mIndex);
	void SetAxisLock(physx::PxRigidDynamic* Actor, PhysData* Data);
private:
	physx::PxPhysics*	m_Phys;
	physx::PxScene*		m_Scene;
	physx::PxCooking*	m_Cooking;
};

