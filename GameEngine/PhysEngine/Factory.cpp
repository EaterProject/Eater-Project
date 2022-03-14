#include "Factory.h"
#include "PxPhysicsAPI.h"
#include "PhysData.h"


using namespace physx;
Factory::Factory()
{
	m_Phys = nullptr;
	m_Scene = nullptr;
}

Factory::~Factory()
{
	m_Phys = nullptr;
	m_Scene = nullptr;
}

void Factory::Initialize(physx::PxPhysics* Phys, physx::PxScene* Scene, physx::PxCooking* Cooking)
{
	m_Phys		= Phys;
	m_Scene		= Scene;
	m_Cooking	= Cooking;
}

physx::PxShapeFlags Factory::CreateShapeFlag(bool isTrigger)
{
	PxShapeFlags shapeFlags;

	if (isTrigger == true)
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
	}
	
	return shapeFlags;
}

physx::PxShape* Factory::CreateShape(PhysMaterial* m, PhysCollider* c)
{
	///재질을 지정해준다
	PxMaterial* pMaterial = CreateMaterial(m);

	///타입별 shape 생성
	Vector3 Size = c->GetSize();
	PxShape* shape = nullptr;
	switch (c->GetType())
	{
	case PhysCollider::TYPE::BOX:
		shape = CreateBoxCollider(pMaterial, c);
		break;
	case PhysCollider::TYPE::SPHERE:
		shape = CreateSphereCollider(pMaterial, c);
		break;
	case PhysCollider::TYPE::CAPSULE:
		shape = CreateCapsuleCollider(pMaterial, c);
		break;
	case PhysCollider::TYPE::MESH:
		shape = CreateTriangleCollider(pMaterial, c);
		break;
	case PhysCollider::TYPE::TERRAIN:
		shape = CreateHeightFieldCollider(pMaterial, c);
		break;
	}

	
	if (pMaterial != nullptr)
	{
		pMaterial->release();
	}

	return shape;
}

PxShape* Factory::CreateBoxCollider(physx::PxMaterial* m, PhysCollider* c)
{
	PxBoxGeometry Geometry	= PxBoxGeometry(PxReal(c->GetSize().x), PxReal(c->GetSize().y), PxReal(c->GetSize().z));
	PxShape* shape			= m_Phys->createShape(Geometry, *m, true , CreateShapeFlag(c->GetTrigger()));
	return shape;
}

PxMaterial* Factory::CreateMaterial(PhysMaterial* m)
{
	if (m == nullptr)
	{
		return m_Phys->createMaterial(0.5f, 0.5f, 0);
	}
	else
	{
		return m_Phys->createMaterial(m->MT_StaticFriction, m->MT_DynamicFriction, m->MT_Restitution);
	}
}

physx::PxShape* Factory::CreateSphereCollider(physx::PxMaterial* m, PhysCollider* c)
{
	PxSphereGeometry temp = PxSphereGeometry(PxReal(c->GetSize().x));
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}

physx::PxShape* Factory::CreateCapsuleCollider(physx::PxMaterial* m, PhysCollider* c)
{
	PxCapsuleGeometry temp = PxCapsuleGeometry(PxReal(c->GetSize().x), PxReal(c->GetSize().y));
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}

void Factory::CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	if (Data->isDinamic == false) { return; }
	PxRigidDynamic* body = m_Phys->createRigidDynamic(*Tr);
	SetAxisLock(body, Data);

	const PxFilterData triggerFilterData(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	shape->setSimulationFilterData(triggerFilterData);

	body->attachShape(*shape);

	physx::PxVec3 temp = physx::PxVec3(Data->CenterPoint.x, Data->CenterPoint.y, Data->CenterPoint.z);

	PxRigidBodyExt::updateMassAndInertia(*body, Data->MT_Mass,&temp);

	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Data->isKinematic);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !Data->isGrvity);
	

	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;


	m_Scene->addActor(*body);
}

void Factory::CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	if (Data->isDinamic == true) { return; }
	PxRigidStatic* body = m_Phys->createRigidStatic(*Tr);

	//const PxFilterData triggerFilterData(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	//shape->setSimulationFilterData(triggerFilterData);


	body->attachShape(*shape);
	m_Scene->addActor(*body);

	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;
}

void Factory::CreateActoer(PhysData* data)
{
	PhysMaterial* mMaterial = data->Meterial;
	PhysCollider* mCollider = data->Collider;

	///위치,회전을 지정해준다
	PxVec3 Pos				= PxVec3(data->WorldPosition.x, data->WorldPosition.y, data->WorldPosition.z);
	PxQuat Rot				= PxQuat(data->Rotation.x, data->Rotation.y, data->Rotation.z, data->Rotation.w);
	PxTransform  mTransform = PxTransform(Pos,Rot);

	PxShape* shape = CreateShape(mMaterial, mCollider);

	///로컬 포지션을 지정
	shape->setLocalPose(PxTransform(data->CenterPoint.x, data->CenterPoint.y, data->CenterPoint.z));

	///물리 객체 생성
	if (data->isDinamic == true)
	{
		CreateDinamicActor(data, shape, &mTransform);
	}
	else
	{
		CreateStaticActor(data, shape, &mTransform);
	}

	///생성이 되었다면 사용한 물리객체를 삭제해준다
	if (shape != nullptr)
	{
		shape->release();
	}
}


void Factory::SetAxisLock(PxRigidDynamic* Actor, PhysData* Data)
{	
	//회전,위치를 막는다

	PxRigidDynamicLockFlags Flag;
	if((bool)Data->FreezePositon.x == true)
	{
		Flag |=PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
	}

	if ((bool)Data->FreezePositon.y == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
	}
	
	if ((bool)Data->FreezePositon.z == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
	}

	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
	}
	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
	}
	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
	}

	Actor->setRigidDynamicLockFlags(Flag);
}

physx::PxShape* Factory::CreateTriangleCollider(physx::PxMaterial* m, PhysCollider* c)
{ 
	PhysCollider::TriangleMeshData* Triangle = c->GetTriangleMesh();
	int IndexCount = (int)Triangle->IndexListSize;
	int VertexCount = (int)Triangle->VertexListSize;

	PxTriangleMeshDesc meshDesc;
	///버텍스 관련 데이터
	meshDesc.points.count		= Triangle->VertexListSize;
	meshDesc.points.stride		= sizeof(DirectX::SimpleMath::Vector3);
	meshDesc.points.data		= Triangle->VertexList;

	///페이스 관련 데이터
	meshDesc.triangles.count	= Triangle->IndexListSize / 3;
	meshDesc.triangles.stride	= 3 * sizeof(UINT);
	meshDesc.triangles.data		= Triangle->IndexList;

	PxTriangleMesh* triMesh = m_Cooking->createTriangleMesh(meshDesc, m_Phys->getPhysicsInsertionCallback());
	PxTriangleMeshGeometry geom;
	geom.triangleMesh = triMesh;
	physx::PxShape* shape = m_Phys->createShape(geom, *m);

	return shape;
}

physx::PxShape* Factory::CreateHeightFieldCollider(physx::PxMaterial* m, PhysCollider* c)
{
	PhysCollider::TriangleMeshData* Triangle = c->GetTriangleMesh();
	int IndexCount = (int)Triangle->IndexListSize;
	int VertexCount = (int)Triangle->VertexListSize;

	
	PxHeightFieldDesc HeightDesc;
	HeightDesc.setToDefault();

	//PxHeightFieldGeometry(HeightDesc);

	//m_Cooking->createHeightField(HeightDesc);


	return nullptr;
}



