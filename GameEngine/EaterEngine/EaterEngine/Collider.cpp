#include "Collider.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "PhysX_API.h"
#include "Transform.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "GameObject.h"

Collider::Collider()
{
	mPhysData		= PhysX_Create_Data();
	mDebugCollider  = new ColliderData();
	
}

Collider::~Collider()
{
	PhysX_Delete_Actor(mPhysData);

	//delete mPhysData;
	delete mDebugCollider;
}

void Collider::Start()
{
	mPhysData->EaterObj = gameobject;
	mTransform = gameobject->GetTransform();
	CreatePhys();
}

void Collider::PhysicsUpdate()
{
	if (mTransform->Position != mPhysData->WorldPosition)
	{
		mPhysData->SetWorldPosition(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
		mTransform->Position = mPhysData->WorldPosition;
	}
	PhysX_Update_Actor(mPhysData);

	float CenterX = mPhysData->CenterPoint.x;
	float CenterY = mPhysData->CenterPoint.y;
	float CenterZ = mPhysData->CenterPoint.z;

	mTransform->Position.x = mPhysData->WorldPosition.x;
	mTransform->Position.y = mPhysData->WorldPosition.y;
	mTransform->Position.z = mPhysData->WorldPosition.z;

	mTransform->Q_Rotation.x = mPhysData->Rotation.x;
	mTransform->Q_Rotation.y = mPhysData->Rotation.y;
	mTransform->Q_Rotation.z = mPhysData->Rotation.z;
	mTransform->Q_Rotation.w = mPhysData->Rotation.w;

	DebugCollider();
}



void Collider::SetBoxCollider(float Size_x, float Size_y, float Size_z)
{
	mPhysData->mCollider->SetBoxCollider(Size_x,Size_y,Size_z);
}

void Collider::SetBoxCollider(float Radius)
{
	mPhysData->mCollider->SetBoxCollider(Radius);
}

void Collider::SetSphereCollider(float Radius)
{
	mPhysData->mCollider->SetSphereCollider(Radius);
}

void Collider::SetCapsuleCollider(float Radius, float Height)
{
	mPhysData->mCollider->SetCapsuleCollider(Radius,Height);
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
	mPhysData->mCollider->SetTrigger(trigger);
}

void Collider::SetMaterial_Static(float Static)
{
	mPhysData->mMeterial->MT_StaticFriction = Static;
}

void Collider::SetMaterial_Dynamic(float Dynamic)
{
	mPhysData->mMeterial->MT_DynamicFriction = Dynamic;
}

void Collider::SetMaterial_Restitution(float Restitution)
{
	mPhysData->mMeterial->MT_Restitution = Restitution;
}

PhysCollider* Collider::GetCollider()
{
	return mPhysData->mCollider;
}

void Collider::DebugCollider()
{
	Vector3 Pos = gameobject->transform->Position + mPhysData->CenterPoint;
	Vector4 Rot = mPhysData->Rotation;
	Vector3 Scl = mPhysData->mCollider->GetSize();

	if (mPhysData->isDinamic == false) 
	{
		SimpleMath::Matrix PosXM = SimpleMath::Matrix::CreateTranslation(Pos + mPhysData->CenterPoint);
		SimpleMath::Matrix RotXM = CreateXMRot4x4();
		SimpleMath::Matrix SclXM = SimpleMath::Matrix::CreateScale(Scl);

		mDebugCollider->ColliderWorld = SclXM * RotXM* PosXM;
		mDebugCollider->ColliderColor = { 1,0,0,1 };
		gameobject->OneMeshData->Collider_Data = mDebugCollider;
	}
	else
	{
		SimpleMath::Matrix PosXM = SimpleMath::Matrix::CreateTranslation(Pos);
		SimpleMath::Matrix RotXM = XMMatrixRotationQuaternion(gameobject->transform->Q_Rotation);
		SimpleMath::Matrix SclXM = SimpleMath::Matrix::CreateScale(Scl);

		mDebugCollider->ColliderWorld = SclXM * RotXM * PosXM;
		mDebugCollider->ColliderColor = { 1,0,0,1 };
		gameobject->OneMeshData->Collider_Data = mDebugCollider;
	}
}

bool Collider::CreatePhys()
{
	if (isCreate == false)
	{
		mPhysData->WorldPosition	= gameobject->transform->Position;
		Vector3 Rot					= gameobject->transform->Rotation;

		Rigidbody* mRigidbody = gameobject->GetComponent<Rigidbody>();
		if (mRigidbody != nullptr)
		{
			mPhysData->isDinamic = true;
			mRigidbody->isCreate = true;
		}
		else
		{
			mPhysData->isDinamic = false;
		}
		
		Quaternion Q_Rot = SimpleMath::Quaternion::CreateFromRotationMatrix(CreateXMRot4x4());
		Transform* mTransform = gameobject->GetTransform();
		mPhysData->SetWorldPosition(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
		mPhysData->Rotation = Q_Rot;
		mTransform->isRigid = true;
		mTransform->Q_Rotation = Q_Rot;
		PhysX_Create_Actor(mPhysData);
		isCreate = true;
		return true;
	}
	else
	{
		return false;
	}
}

float Collider::GetMaterial_Static()
{
	return mPhysData->mMeterial->MT_StaticFriction;
}

float Collider::GetMaterial_Dynamic()
{
	return mPhysData->mMeterial->MT_DynamicFriction;
}

float Collider::GetMaterial_Restitution()
{
	return mPhysData->mMeterial->MT_Restitution;
}

bool Collider::GetTriggerEnter()
{
	return mPhysData->GetTriggerEnter();
}

bool Collider::GetTriggerStay()
{
	return mPhysData->GetTriggerStay();
}

bool Collider::GetTriggerExit()
{
	return mPhysData->GetTriggerExit();
}

int Collider::GetTriggerCount()
{
	return (int)mPhysData->TriggerList.size();
}

GameObject* Collider::GetTriggerObject()
{
	if (mPhysData->TriggerList[0] != nullptr)
	{
		return reinterpret_cast<GameObject*>(mPhysData->EaterObj);
	}
	else 
	{
		return nullptr;
	}
}

void Collider::SetCenter(float x, float y, float z)
{
	mPhysData->CenterPoint = { x,y,z };
}

DirectX::SimpleMath::Matrix Collider::CreateXMRot4x4()
{
	float radX = gameobject->transform->Rotation.x * 3.141592f / 180;
	float radY = gameobject->transform->Rotation.y * 3.141592f / 180;
	float radZ = gameobject->transform->Rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
	return _R * _Y * _P;
}