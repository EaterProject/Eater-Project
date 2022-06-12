#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"

class Rigidbody;
class PhysCollider;
class PhysData;
class PhysMaterial;
class TriangleMeshData;
class Transform;

class Collider : public Component
{
public:
	EATER_ENGINEDLL Collider();
	EATER_ENGINEDLL ~Collider();
	void Start() override;
	void PhysicsUpdate() override;
	void Debug() override;
public:
	///Set
	EATER_ENGINEDLL void SetBoxCollider(float Size_x, float Size_y, float Size_z);		//네모 모양에 콜라이더 생성
	EATER_ENGINEDLL void SetBoxCollider(float Radius);									//네모 콜라이더 생성
	EATER_ENGINEDLL void SetSphereCollider(float Radius);								//원 콜라이더 생성
	EATER_ENGINEDLL void SetCapsuleCollider(float Radius, float Height);				//캡슐 콜라이더 생성
	EATER_ENGINEDLL void SetTriangleCollider(std::string MeshName);						//터레인 정보에 따라 콜라이더 생성
	EATER_ENGINEDLL void SetCenter(float x, float y, float z);							//중심점 설정
	EATER_ENGINEDLL void SetTrigger(bool Trigger);			
	//트리거 설정
	EATER_ENGINEDLL void SetMaterial_Static(float Static);					//재질 설정
	EATER_ENGINEDLL void SetMaterial_Dynamic(float Dynamic);				//재질 설정
	EATER_ENGINEDLL void SetMaterial_Restitution(float Restitution);		//재질 설정

	///Get
	EATER_ENGINEDLL PhysCollider* GetCollider();
	EATER_ENGINEDLL bool CreatePhys();
	EATER_ENGINEDLL std::string GetTriangleName();


	EATER_ENGINEDLL float GetMaterial_Static();
	EATER_ENGINEDLL float GetMaterial_Dynamic();
	EATER_ENGINEDLL float GetMaterial_Restitution();

	EATER_ENGINEDLL bool GetTriggerEnter();
	EATER_ENGINEDLL bool GetTriggerStay();
	EATER_ENGINEDLL bool GetTriggerExit();
private:
	void FindPhysFunctionEnter(PhysData* Data, unsigned int Type);
	void FindPhysFunctionStay(PhysData* Data, unsigned int Type);
	void FindPhysFunctionExit(PhysData* Data, unsigned int Type);
	DirectX::SimpleMath::Matrix CreateXMRot4x4();
private:
	PhysData*		mPhysData;
	Rigidbody*		mRigidbody;
	Transform*		mTransform;
	std::string TriangleName = "";
	bool isCreate	= false;
	friend Rigidbody;
	friend Transform;
};

