#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysData.h"
#include "PhysX_API.h"
#include "PhysData.h"
#include "EngineData.h"
#include "Collider.h"

Rigidbody::Rigidbody()
{
	//컨퍼넌트 실행 순서를 변경
	Component::Awake_Order				= Component::FUNCTION_ORDER_LAST;
	Component::TransformUpdate_Order	= Component::FUNCTION_ORDER_FIRST;
	Component::SetUp_Order				= Component::FUNCTION_ORDER_LAST;
}

Rigidbody::~Rigidbody()
{
	
}

void Rigidbody::Awake()
{
	Collider* mCollider = gameobject->GetComponent<Collider>();
	mTransform			= gameobject->GetTransform();
	if (mCollider != nullptr)
	{
		RigidbodyData = mCollider->mPhysData;

		RigidbodyData->isGravity	= isGravity;
		RigidbodyData->isKinematic	= isKinematic;
		RigidbodyData->MT_Mass = Mass;
		RigidbodyData->SetLockAxis_Position(FreezePos[0], FreezePos[1], FreezePos[2]);
		RigidbodyData->SetLockAxis_Rotation(FreezeRot[0], FreezeRot[1], FreezeRot[2]);
	}
}

void Rigidbody::SetUp()
{
	/// 시작 값을 넣어준다
	//Tr = gameobject->GetTransform();
}

void Rigidbody::PhysicsUpdate()
{
	

}

 bool Rigidbody::GetTriggerEnter()
{
	 return RigidbodyData->GetTriggerEnter();
}

 bool Rigidbody::GetTriggerStay()
{
	return RigidbodyData->GetTriggerStay();
}

 bool Rigidbody::GetTriggerExit()
{
	return RigidbodyData->GetTriggerExit();
}

int Rigidbody::GetTriggerCount()
{
	return RigidbodyData->TriggerCount;
}

void Rigidbody::SetGravity(bool grvity)
{
	if (RigidbodyData == nullptr)
	{
		isGravity = grvity;
	}
	else 
	{
		RigidbodyData->isGravity = grvity;
	}
}

bool Rigidbody::GetGravity()
{
	return RigidbodyData->isGravity;
}


void Rigidbody::SetKinematic(bool kinematic)
{
	if (RigidbodyData == nullptr)
	{
		isKinematic = kinematic;
	}
	else
	{
		RigidbodyData->isKinematic = kinematic;
	}
}

bool Rigidbody::GetKinematic()
{
	return RigidbodyData->isKinematic;
}

void Rigidbody::SetMass(float mass)
{
	if (RigidbodyData == nullptr)
	{
		Mass = mass;
	}
	else
	{
		RigidbodyData->MT_Mass = mass;
	}
}

float Rigidbody::GetMass()
{
	return RigidbodyData->MT_Mass;
}

void Rigidbody::SetFreezePosition(bool x, bool y, bool z)
{
	if (RigidbodyData == nullptr)
	{
		FreezePos[0] = x;
		FreezePos[1] = y;
		FreezePos[2] = z;
	}
	else
	{
		RigidbodyData->SetLockAxis_Position(x, y, z);
	}
}

Vector3 Rigidbody::GetFreezePosition()
{
	return RigidbodyData->GetLockAxis_Position();
}

void Rigidbody::SetFreezeRotation(bool x, bool y, bool z)
{
	if (RigidbodyData == nullptr)
	{
		FreezeRot[0] = x;
		FreezeRot[1] = y;
		FreezeRot[2] = z;
	}
	else
	{
		RigidbodyData->SetLockAxis_Rotation(x, y, z);
	}
}

Vector3 Rigidbody::GetFreezeRotation()
{
	return RigidbodyData->GetLockAxis_Rotation();
}

void Rigidbody::SetVelocity(float x, float y, float z)
{
	RigidbodyData->SetVelocity(x, y, z);
}

Vector3 Rigidbody::GetVelocity()
{
	return RigidbodyData->GetVelocity();
}

 void Rigidbody::SetAddForce(float x, float y, float z)
 {
	 RigidbodyData->AddForce(x, y, z);
 }

 void Rigidbody::SetCenterPoint(float x, float y, float z)
 {
	 RigidbodyData->CenterPoint = {x,y,z}; 
 }

 GameObject* Rigidbody::GetTriggerObject()
 {
	 //if (RigidbodyData->TriggerList[0] != nullptr)
	 //{
	//	 return reinterpret_cast<GameObject*>(RigidbodyData->EaterObj);
	 //}
	 //else
	 //{
	//	 return nullptr;
	 //}
	 return nullptr;
 }





