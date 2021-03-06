#include "Collider.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "PhysX_API.h"
#include "Transform.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "GameObject.h"
#include "DebugManager.h"
#include "PhysCollider.h"

Collider::Collider()
{
	mPhysData = PhysX_Create_Data();
}

Collider::~Collider()
{
	if (mPhysData != nullptr)
	{
		PhysX_Delete_Actor(mPhysData);
	}
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

	Vector3 position = mTransform->GetPosition();

	if (position != mPhysData->WorldPosition)
	{
		mPhysData->SetWorldPosition(position.x, position.y, position.z);
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

		mTransform->SetPosition(mPhysData->WorldPosition);

		//mTransform->Q_Rotation.x = mPhysData->Rotation.x;
		//mTransform->Q_Rotation.y = mPhysData->Rotation.y;
		//mTransform->Q_Rotation.z = mPhysData->Rotation.z;
		//mTransform->Q_Rotation.w = mPhysData->Rotation.w;
	}

	//충돌 함수 호출
	FindPhysFunctionEnter(mPhysData, PHYS_TRIIGER_ENTER);
	FindPhysFunctionStay(mPhysData, PHYS_TRIIGER_STAY);
	FindPhysFunctionExit(mPhysData, PHYS_TRIIGER_EXIT);
}

void Collider::Debug()
{
	if (mPhysData == nullptr) { return; }
	Vector3 Pos = mTransform->GetPosition() + mPhysData->CenterPoint;
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
			DebugManager::DebugDrawBox(Scl, mTransform->GetRotation_Q(), Pos + mPhysData->CenterPoint, Vector3(1, 0, 0));
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

void Collider::SetTriangleCollider(std::string MeshName)
{
	PhysX_Delete_Actor(mPhysData);
	TriangleName = MeshName;
	
	//버퍼에서 매쉬 가져오기
	ColliderBuffer* data = LoadManager::GetColliderBuffer(MeshName);
	int IndexSize	= data->IndexArrayCount;
	int VertexSize	= data->VertexArrayCount;
	if (data == nullptr) { return; }

	//생성
	mPhysData = PhysX_Create_Data();
	
	mPhysData->mCollider->CreateTriangle(IndexSize, VertexSize, data->IndexArray, data->VertexArray);

	isCreate = false;
	CreatePhys();
}

void Collider::SetTrigger(bool trigger)
{
	mPhysData->SetTrigger(trigger);
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

void Collider::DeletePhysCollider()
{
	PhysX_Delete_Actor(mPhysData);
	mPhysData = nullptr;
}

void Collider::FindPhysFunctionEnter(PhysData* Data, unsigned int Type)
{
	int MaxCount = mPhysData->Enter_Count;
	int NowCount = MaxCount;

	if (MaxCount <= 0) { return; }
	if (Data == nullptr) { return; }

	//충돌된 오브젝트에 대한 함수 실행
	for (int i = 0; i < 10; i++)
	{
		PhysData* TargetData = Data->TriggerEnter_List[i];
		if (TargetData == nullptr) { continue; }
		if ((unsigned long long)TargetData->EaterObj == 0xdddddddddddddddd)
		{
			Data->TriggerEnter_List[i] = nullptr;
			continue;
		}

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
	if (Data == nullptr) { return; }

	for (int i = 0; i < 10; i++)
	{
		PhysData* TargetData = Data->TriggerStay_List[i];

		if (TargetData == nullptr) { continue; }
		if ((unsigned long long)TargetData->EaterObj == 0xdddddddddddddddd)
		{
			Data->TriggerStay_List[i] = nullptr;
			continue;
		}

		GameObject* Object = reinterpret_cast<GameObject*>(TargetData->EaterObj);
		gameobject->PlayPhysFunction(Object, Type);
	}
}

void Collider::FindPhysFunctionExit(PhysData* Data, unsigned int Type)
{
	int MaxCount = mPhysData->Exit_Count;
	int NowCount = MaxCount;

	if (MaxCount <= 0) { return; }
	if (Data == nullptr) { return; }

	for (int i = 0; i < 10; i++)
	{
		PhysData* TargetData = Data->TriggerExit_List[i];

		if (TargetData == nullptr) { continue; }
		if ((unsigned long long)TargetData->EaterObj == 0xdddddddddddddddd)
		{
			Data->TriggerExit_List[i] = nullptr;
			continue;
		}

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
		mPhysData->WorldPosition	= gameobject->transform->GetPosition();
		Vector3 Rot					= gameobject->transform->GetRotation();

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
		Vector3 position = mTransform->GetPosition();

		mPhysData->SetWorldPosition(position.x, position.y, position.z);
		mPhysData->Rotation		= Q_Rot;
		mTransform->SetRotate(Q_Rot);

		PhysX_Create_Actor(mPhysData);

		isCreate = true;
		return true;
	}
	else
	{
		return false;
	}
}

 std::string Collider::GetTriangleName()
{
	 return TriangleName;
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

void Collider::SetCenter(float x, float y, float z)
{
	mPhysData->CenterPoint = { x,y,z };
}

DirectX::SimpleMath::Matrix Collider::CreateXMRot4x4()
{
	DirectX::SimpleMath::Vector3 rot = gameobject->transform->GetRotation();

	float radX = rot.x * 3.141592f / 180;
	float radY = rot.y * 3.141592f / 180;
	float radZ = rot.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);
	return _R * _Y * _P;
}