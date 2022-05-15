#pragma once

#include "EaterEngineDLL.h"
#include "Component.h"
#include <string>

class Transform;
class PhysData;
class DebugBoxData;
class Collider;

class Rigidbody : public Component
{
public:
	EATER_ENGINEDLL Rigidbody();
	EATER_ENGINEDLL ~Rigidbody();

	
	void Awake();
	virtual void SetUp();
	virtual void PhysicsUpdate();

	EATER_ENGINEDLL bool GetTriggerEnter();
	EATER_ENGINEDLL bool GetTriggerStay();
	EATER_ENGINEDLL bool GetTriggerExit();
	EATER_ENGINEDLL int GetTriggerCount();

	//중력 적용 여부
	EATER_ENGINEDLL void SetGravity(bool grvity);
	EATER_ENGINEDLL bool GetGravity();
	//
	EATER_ENGINEDLL void SetKinematic(bool kinematic);
	EATER_ENGINEDLL bool GetKinematic();
	//무게를 지정한다
	EATER_ENGINEDLL void SetMass(float mass);
	EATER_ENGINEDLL float GetMass();

	//원하는 축의 방향이동을 고정 시킨다
	EATER_ENGINEDLL void SetFreezePosition(bool x,bool y,bool z);
	EATER_ENGINEDLL Vector3 GetFreezePosition();

	//원하는 축의 방향회전을 고정 시킨다 
	EATER_ENGINEDLL void SetFreezeRotation(bool x,bool y,bool z);
	EATER_ENGINEDLL Vector3 GetFreezeRotation();
	//rigdbody를 이용해서 움직임
	EATER_ENGINEDLL void SetVelocity(float x, float y, float z);
	
	EATER_ENGINEDLL Vector3 GetVelocity();

	//방향대로 힘을 준다
	EATER_ENGINEDLL void SetAddForce(float x, float y, float z);
	//초기 위치값에서 센터좌표를 다시 지정한다 
	EATER_ENGINEDLL void SetCenterPoint(float x, float y, float z);
	EATER_ENGINEDLL GameObject* GetTriggerObject();
	bool isCreate = false;
private:
	bool isGravity		= true;
	bool isKinematic	= false;
	float Mass			= 1;
	bool FreezePos[3];
	bool FreezeRot[3];
private:

	Transform*		mTransform;
	PhysData*		RigidbodyData;
	DebugBoxData*	Collider_Data;
	std::string		ColliderName;
};

