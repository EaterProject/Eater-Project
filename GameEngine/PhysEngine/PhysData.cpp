#include "PhysData.h"
#include "PhysMaterial.h"
#include "PhysCollider.h"

PhysData::PhysData()
{
	//디폴트 값 초기화
	WorldPosition	= { 0.0f,0.0f,0.0f};
	LocalPosition	= { 0.0f,0.0f,0.0f};
	Rotation		= { 0.0f,0.0f,0.0f,1.0f };
	FreezePositon	= { 0.0f,0.0f,0.0f};
	FreezeRotaticon = { 0.0f,0.0f,0.0f};
	Velocity		= { 0.0f,0.0f,0.0f};
	CenterPoint		= { 0.0f,0.0f,0.0f};
	PhysX_Velocity  = { 0.0f,0.0f,0.0f};

	MT_Mass = 1.0f;

	isDinamic	= false;
	isGravity	= true;
	isKinematic = false;
	isVelocity	= false;
	isForce		= false;
	isPosition  = false;
	isTrigger	= false;

	OnTriggerEnter = false;
	OnTriggerStay = false;
	OnTriggerExit = false;

	ActorObj	= nullptr;
	mMeterial	= new PhysMaterial();
	mCollider	= new PhysCollider();
}

PhysData::~PhysData()
{
	ActorObj = nullptr;
}

void PhysData::SetWorldPosition(float x, float y, float z)
{
	WorldPosition.x = x;
	WorldPosition.y = y;
	WorldPosition.z = z;
	isPosition = true;
}

void PhysData::SetLocalPosition(float x, float y, float z)
{
	LocalPosition.x = x;
	LocalPosition.y = y;
	LocalPosition.z = z;
}

void PhysData::SetRotation(float x, float y, float z)
{
	Rotation.x	= x;
	Rotation.y	= y;
	Rotation.z	= z;
}

void PhysData::SetRotate(float x, float y, float z)
{
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
}

void PhysData::AddForce(float x, float y, float z)
{
	Force.x = x;
	Force.y = y;
	Force.z = z;
	isForce = true;
}

void PhysData::SetVelocity(float x, float y, float z)
{
	Velocity.x = x;
	Velocity.y = y;
	Velocity.z = z;
	isVelocity = true;
}

void PhysData::SetTrigger(bool mTrigger)
{
	isTrigger = mTrigger;
}

Vector3 PhysData::GetVelocity()
{
	return PhysX_Velocity;
}

bool PhysData::GetTriggerEnter()
{
	bool On = OnTriggerEnter;
	if (On == true)
	{
		OnTriggerEnter	= false;
		OnTriggerStay	= true;
	}
	return On;
}

bool PhysData::GetTriggerStay()
{
	bool on = OnTriggerExit;
	if (on == true)
	{
		OnTriggerExit = false;
		OnTriggerStay = false;
	}
	return on;
}

bool PhysData::GetTriggerExit()
{
	bool off = OnTriggerExit;
	if (OnTriggerExit == true)
	{
		OnTriggerExit = false;
	}
	return off;
}

void PhysData::SetLockAxis_Position(bool x, bool y, bool z)
{
	FreezePositon.x = x;
	FreezePositon.y = y;
	FreezePositon.z = z;
}

Vector3 PhysData::GetLockAxis_Position()
{
	return Vector3(FreezePositon.x, FreezePositon.y, FreezePositon.z);
}

void PhysData::SetLockAxis_Rotation(bool x, bool y, bool z)
{
	FreezeRotaticon.x = x;
	FreezeRotaticon.y = y;
	FreezeRotaticon.z = z;
}

Vector3 PhysData::GetLockAxis_Rotation()
{
	return Vector3(FreezeRotaticon.x, FreezeRotaticon.y, FreezeRotaticon.z);
}

void PhysData::PushTriggerEnter_Data(PhysData* Data)
{
	//현재 충돌된 오브젝트가 들어온다
	OnTriggerEnter = true;
	Enter_Count++;
	//충돌된 리스트에서 빈자리를 찾는다
	for (int i = 0; i < 10; i++)
	{
		if (TriggerEnter_List[i] == nullptr)
		{
			TriggerEnter_List[i] = Data;
			TriggerCount++;
			break;
		}
	}

	OnTriggerStay = true;
}

void PhysData::PushTriggerExit_Data(PhysData* Data)
{
	OnTriggerStay = false;
	OnTriggerExit = true;
	Exit_Count++;
	for (int i = 0; i < 10; i++)
	{
		if (TriggerExit_List[i] == nullptr)
		{
			TriggerExit_List[i] = Data;
			TriggerCount--;
			break;
		}
	}
}





