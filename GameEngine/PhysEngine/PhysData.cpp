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
	TriggerList.resize(0);
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

Vector3 PhysData::GetVelocity()
{
	return PhysX_Velocity;
}

bool PhysData::GetTriggerEnter()
{
	bool On = OnTriggerEnter;
	if (OnTriggerEnter == true)
	{
		OnTriggerEnter = false;
	}
	return On;
}

bool PhysData::GetTriggerStay()
{
	if (TriggerCount != 0)
	{
		return true;
	}
	return false;
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


PhysRayCast::PhysRayCast()
{


}

PhysRayCast::~PhysRayCast()
{


}
