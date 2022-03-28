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

	//Ÿ���� ���� �Ѵ�(static , Dinamic)
	EATER_ENGINEDLL void SetType(bool Dinamic);
	//�߷� ���� ����
	EATER_ENGINEDLL void SetGrvity(bool grvity);
	//
	EATER_ENGINEDLL void SetKinematic(bool kinematic);
	//���Ը� �����Ѵ�
	EATER_ENGINEDLL void SetMass(float mass);

	//���ϴ� ���� �����̵��� ���� ��Ų��
	EATER_ENGINEDLL void SetFreezePosition(bool x,bool y,bool z);
	//���ϴ� ���� ����ȸ���� ���� ��Ų�� 
	EATER_ENGINEDLL void SetFreezeRotation(bool x,bool y,bool z);
	//rigdbody�� �̿��ؼ� ������
	EATER_ENGINEDLL void SetVelocity(float x, float y, float z);
	
	EATER_ENGINEDLL Vector3 GetVelocity();

	//������ ���� �ش�
	EATER_ENGINEDLL void SetAddForce(float x, float y, float z);
	//�ʱ� ��ġ������ ������ǥ�� �ٽ� �����Ѵ� 
	EATER_ENGINEDLL void SetCenterPoint(float x, float y, float z);

	bool isCreate = false;
private:
	Transform*		mTransform;
	PhysData*		RigidbodyData;
	ColliderData*	Collider_Data;
	std::string		ColliderName;
};

