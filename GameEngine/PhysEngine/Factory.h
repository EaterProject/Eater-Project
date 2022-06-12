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
class TriangleMeshData;

class Phys_Collider_Capsule;
class Phys_Collider_Sphere;
class Phys_Collider_Triangle;
class Phys_Collider_Box;


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
	physx::PxShape*		CreateShape(PhysMaterial* m, PhysCollider* c, bool isTrigger);
private:
	///콜라이더 생성
	physx::PxShape*		CreateBoxCollider(physx::PxMaterial* m, Phys_Collider_Box* c, bool isTrigger);
	physx::PxShape*		CreateSphereCollider(physx::PxMaterial* m, Phys_Collider_Sphere* c, bool isTrigger);
	physx::PxShape*		CreateCapsuleCollider(physx::PxMaterial* m, Phys_Collider_Capsule* c, bool isTrigger);
	physx::PxShape*		CreateTriangleCollider(physx::PxMaterial* m , Phys_Collider_Triangle* c);
private:
	///재질 생성
	physx::PxMaterial*	CreateMaterial(PhysMaterial* m);
private:
	///Actor 셋팅
	void CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
	void CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
private:
	void SetAxisLock(physx::PxRigidDynamic* Actor, PhysData* Data);
private:
	physx::PxPhysics*	m_Phys;
	physx::PxScene*		m_Scene;
	physx::PxCooking*	m_Cooking;
};

