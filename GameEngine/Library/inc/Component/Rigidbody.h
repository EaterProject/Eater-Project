#pragma once

#include "EaterEngineDLL.h"
#include "Component.h"
#include <string>

class Transform;
class PhysData;
class ColliderData;
class Collider;

class Rigidbody : public Component
{
public:
	EATER_ENGINEDLL Rigidbody();
	EATER_ENGINEDLL ~Rigidbody();

	
	void Awake();
	virtual void Start();
	virtual void SetUp();
	virtual void PhysicsUpdate();

	EATER_ENGINEDLL void SetRotation(float x, float y, float z, float w);
	EATER_ENGINEDLL void SetRotation(float x, float y, float z);
	EATER_ENGINEDLL void SetPosition(float x, float y, float z);


	EATER_ENGINEDLL bool GetTriggerEnter();
	EATER_ENGINEDLL bool GetTriggerStay();
	EATER_ENGINEDLL bool GetTriggerExit();
	EATER_ENGINEDLL int GetTriggerCount();

	//타입을 정의 한다(static , Dinamic)
	EATER_ENGINEDLL void SetType(bool Dinamic);
	//중력 적용 여부
	EATER_ENGINEDLL void SetGrvity(bool grvity);
	//
	EATER_ENGINEDLL void SetKinematic(bool kinematic);
	//무게를 지정한다
	EATER_ENGINEDLL void SetMass(float mass);

	//원하는 축의 방향이동을 고정 시킨다
	EATER_ENGINEDLL void SetFreezePosition(bool x,bool y,bool z);
	//원하는 축의 방향회전을 고정 시킨다 
	EATER_ENGINEDLL void SetFreezeRotation(bool x,bool y,bool z);
	//rigdbody를 이용해서 움직임
	EATER_ENGINEDLL void SetVelocity(float x, float y, float z);
	
	EATER_ENGINEDLL Vector3 GetVelocity();

	//방향대로 힘을 준다
	EATER_ENGINEDLL void SetAddForce(float x, float y, float z);
	//초기 위치값에서 센터좌표를 다시 지정한다 
	EATER_ENGINEDLL void SetCenterPoint(float x, float y, float z);

	bool isCreate = false;
private:
	Transform*		mTransform;
	PhysData*		RigidbodyData;
	ColliderData*	Collider_Data;
	std::string		ColliderName;
};

