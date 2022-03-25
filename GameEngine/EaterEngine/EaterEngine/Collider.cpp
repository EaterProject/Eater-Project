#include "Collider.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "PhysX_API.h"
#include "Transform.h"
#include "EngineData.h"
#include "LoadManager.h"

Collider::Collider()
{
	Component::Start_Order = Component::FUNCTION_ORDER_LAST;
	mPhysData		= new PhysData();
	mColliderData	= new PhysCollider();
	mDebugCollider  = new ColliderData();
}

Collider::~Collider()
{
	PhysX_Delete_Actor(mPhysData);

	if (mMaterial != nullptr)
	{
		delete mMaterial;
	}

	delete mColliderData;
	delete mPhysData;
	delete mDebugCollider;
}

void Collider::Awake()
{
	//실행되기전에 
	mRigidbody =  gameobject->GetComponent<Rigidbody>();
	mTransform =  gameobject->GetTransform();
}

void Collider::Start()
{
	//콜라이더만 있다면 스테틱 Rigidbody도 있다면 다이나믹
	if (mRigidbody != nullptr)
	{
		mPhysData->isDinamic = true;
	}

	//메테리얼을 설정했다면 넣어준다
	if (mMaterial != nullptr)
	{
		mPhysData->Meterial = mMaterial;
	}

	mPhysData->Collider = mColliderData;
	mPhysData->WorldPosition = mTransform->Position;
	Vector3 Rot = mTransform->Rotation;
	mPhysData->SetRotate(Rot.x, Rot.y, Rot.z);
	PhysX_Create_Actor(mPhysData);
}

void Collider::Update()
{
	
}

void Collider::SetBoxCollider(float Size_x, float Size_y, float Size_z)
{
	mColliderData->SetBoxCollider(Size_x, Size_y, Size_z);
}

void Collider::SetBoxCollider(float Radius)
{
	mColliderData->SetBoxCollider(Radius);
}

void Collider::SetSphereCollider(float Radius)
{
	mColliderData->SetSphereCollider(Radius);
}

void Collider::SetCapsuleCollider(float Radius, float Height)
{
	mColliderData->SetCapsuleCollider(Radius,Height);
}

void Collider::SetMeshCollider(std::string MeshName)
{
	//PhysCollider::TriangleMeshData* Triangle = mColliderData->CreateTriangle();
	//ModelData* data = LoadManager::GetMesh(MeshName);
	//
	//int IndexSize = data->TopMeshList[0]->m_OriginIndexListCount;
	//int VertexSize = data->TopMeshList[0]->m_OriginVertexListCount;
	//
	//Triangle->VertexList = data->TopMeshList[0]->m_OriginVertexList;
	//Triangle->VertexListSize = VertexSize;
	//
	//Triangle->IndexList = data->TopMeshList[0]->m_OriginIndexList;
	//Triangle->IndexListSize = IndexSize;
}

void Collider::SetTerrainCollider(std::string MeshName)
{
	//PhysCollider::TriangleMeshData* Triangle = mColliderData->CreateTriangle();
	//ModelData* data = LoadManager::GetMesh(MeshName);
	//
	//int IndexSize = data->TopMeshList[0]->m_OriginIndexListCount;
	//int VertexSize = data->TopMeshList[0]->m_OriginVertexListCount;
	//
	//Triangle->VertexList = data->TopMeshList[0]->m_OriginVertexList;
	//Triangle->VertexListSize = VertexSize;
	//
	//Triangle->IndexList = data->TopMeshList[0]->m_OriginIndexList;
	//Triangle->IndexListSize = IndexSize;
}

void Collider::SetTrigger(bool trigger)
{
	mColliderData->SetTrigger(trigger);
}

void Collider::SetMaterial(PhysMaterial* m)
{
	mMaterial = m;
}

PhysCollider* Collider::GetCollider()
{
	return mColliderData;
}

void Collider::DebugCollider()
{
	Vector3 Pos = mPhysData->WorldPosition;
	Vector4 Rot = mPhysData->Rotation;
	Vector3 Scl = mColliderData->GetSize();

	//mDebugCollider->ColliderWorld = *(mTransform->GetWorld());
	//mDebugCollider->ColliderColor = { 1,0,0,1 };
	//gameobject->OneMeshData->Collider_Data = mDebugCollider;
}

void Collider::SetCenter(float x, float y, float z)
{
	mColliderData->SetCenter(x, y, z);
}


