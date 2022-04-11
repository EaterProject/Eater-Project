#include "Collider.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "PhysX_API.h"
#include "Transform.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "GameObject.h"
#include "DebugManager.h"

Collider::Collider()
{
	mPhysData		= PhysX_Create_Data();
}

Collider::~Collider()
{
	PhysX_Delete_Actor(mPhysData);
}

void Collider::Start()
{
	mPhysData->EaterObj = gameobject;
	mTransform = gameobject->GetTransform();
	CreatePhys();
}

void Collider::PhysicsUpdate()
{
	if (mPhysData == nullptr) { return; }

	if (mTransform->Position != mPhysData->WorldPosition)
	{
		mPhysData->SetWorldPosition(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
		mTransform->Position = mPhysData->WorldPosition;
	}

	//if (mTransform->Q_Rotation != mPhysData->Rotation)
	//{
	//	mPhysData->SetRotation(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
	//}
	
	PhysX_Update_Actor(mPhysData);
	if (mPhysData->isDinamic == true)
	{
		float CenterX = mPhysData->CenterPoint.x;
		float CenterY = mPhysData->CenterPoint.y;
		float CenterZ = mPhysData->CenterPoint.z;

		mTransform->Position.x = mPhysData->WorldPosition.x;
		mTransform->Position.y = mPhysData->WorldPosition.y;
		mTransform->Position.z = mPhysData->WorldPosition.z;

		//mTransform->Q_Rotation.x = mPhysData->Rotation.x;
		//mTransform->Q_Rotation.y = mPhysData->Rotation.y;
		//mTransform->Q_Rotation.z = mPhysData->Rotation.z;
		//mTransform->Q_Rotation.w = mPhysData->Rotation.w;
	}


	
	DebugCollider();
	//충돌 함수 호출
	FindPhysFunctionEnter(mPhysData, PHYS_TRIIGER_ENTER);
	FindPhysFunctionStay(mPhysData, PHYS_TRIIGER_STAY);
	FindPhysFunctionExit(mPhysData, PHYS_TRIIGER_EXIT);
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

void Collider::SetTriangleCollider(std::string MeshName)
{
	PhysX_Delete_Actor(mPhysData);
	delete mPhysData;
	mPhysData = nullptr;

	mPhysData = PhysX_Create_Data();
	PhysCollider::TriangleMeshData* Triangle = mPhysData->mCollider->CreateTriangle();
	ColliderBuffer* data = LoadManager::GetColliderBuffer(MeshName);
	
	int IndexSize = data->IndexArrayCount;
	int VertexSize = data->VertexArrayCount;
	
	//트라이앵글 셋팅
	Triangle->Name				= MeshName;
	Triangle->VertexList		= data->VertexArray;
	Triangle->VertexListSize	= VertexSize;
	Triangle->CIndexList		= data->FaceArray;
	Triangle->IndexListSize		= IndexSize;

	isCreate = false;
	CreatePhys();
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

	switch (mPhysData->mCollider->GetType())
	{
	case PhysCollider::TYPE::BOX:
	{
		if (mPhysData->isDinamic == false)
		{
			DebugManager::DebugDrawBox(Scl, Rot, Pos + mPhysData->CenterPoint, Vector3(1, 0, 0));
		}
		else
		{
			DebugManager::DebugDrawBox(Scl, gameobject->transform->Q_Rotation, Pos + mPhysData->CenterPoint, Vector3(1, 0, 0));
		}
	}
		break;
	case PhysCollider::TYPE::SPHERE:
	{
		if (mPhysData->isDinamic == false)
		{
			DebugManager::DebugDrawSphere(Scl.x, Pos + mPhysData->CenterPoint, Vector3(1, 0, 0));
		}
		else
		{
			DebugManager::DebugDrawSphere(Scl.x, Pos + mPhysData->CenterPoint, Vector3(1, 0, 0));
		}
	}
		break;
	}
}

void Collider::FindPhysFunctionEnter(PhysData* Data, unsigned int Type)
{
	int MaxCount = mPhysData->Enter_Count;
	int NowCount = MaxCount;
	if (MaxCount <= 0) { return; }

	//충돌된 오브젝트에 대한 함수 실행
	for (int i = 0; i < 10; i++)
	{
		if (Data->TriggerEnter_List[i] == nullptr) { continue; }

		PhysData* TargetData = Data->TriggerEnter_List[i];
		GameObject* Object = reinterpret_cast<GameObject*>(TargetData->EaterObj);
		gameobject->PlayPhysFunction(Object, Type);
		NowCount--;
		Data->TriggerEnter_List[i] = nullptr;
		mPhysData->Enter_Count--;

		//Enter 함수를 호출후 Stay로 옮겨준다
		for (int i = 0; i < 10; i++)
		{
			if (Data->TriggerStay_List[i] == nullptr)
			{
				Data->TriggerStay_List[i] = TargetData;
				mPhysData->Stay_Count++;
				break;
			}
		}

		if (NowCount <= 0)
		{
			break;
		}
	}

	
}

void Collider::FindPhysFunctionStay(PhysData* Data, unsigned int Type)
{
	int MaxCount = mPhysData->Stay_Count;
	int NowCount = MaxCount;

	if (MaxCount <= 0) { return; }

	for (int i = 0; i < 10; i++)
	{
		if (Data->TriggerStay_List[i] == nullptr) { continue; }
		GameObject* Object = reinterpret_cast<GameObject*>(Data->TriggerStay_List[i]->EaterObj);
		gameobject->PlayPhysFunction(Object, Type);
	}
}

void Collider::FindPhysFunctionExit(PhysData* Data, unsigned int Type)
{
	int MaxCount = mPhysData->Exit_Count;
	int NowCount = MaxCount;

	if (MaxCount <= 0) { return; }

	for (int i = 0; i < 10; i++)
	{
		if (Data->TriggerExit_List[i] == nullptr) { continue; }

		PhysData* TargetData = Data->TriggerExit_List[i];
		GameObject* Object = reinterpret_cast<GameObject*>(TargetData->EaterObj);
		gameobject->PlayPhysFunction(Object, Type);

		NowCount--;
		Data->TriggerExit_List[i] = nullptr;
		mPhysData->Exit_Count--;

		for (int i = 0; i < 10; i++)
		{
			if (Data->TriggerStay_List[i] == TargetData)
			{
				Data->TriggerStay_List[i] = nullptr;
				mPhysData->Stay_Count--;
				break;
			}
		}

		if (NowCount <= 0)
		{
			break;
		}
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
			gameobject->GetTransform()->isRigid = true;
		}
		else
		{
			mPhysData->isDinamic = false;
		}
		
		Quaternion Q_Rot = SimpleMath::Quaternion::CreateFromRotationMatrix(CreateXMRot4x4());
		Transform* mTransform = gameobject->GetTransform();
		mPhysData->SetWorldPosition(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
		mPhysData->Rotation = Q_Rot;
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



GameObject* Collider::GetTriggerObject()
{
	//if (mPhysData->TriggerList[0] != nullptr)
	//{
	//	return reinterpret_cast<GameObject*>(mPhysData->TriggerList[0]->EaterObj);
	//}
	//else 
	//{
	//	return nullptr;
	//}
	return nullptr;
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