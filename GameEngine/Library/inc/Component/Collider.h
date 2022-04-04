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
	void Awake();
	void Start();
	void Update();
public:
	///Set
	EATER_ENGINEDLL void SetBoxCollider(float Size_x, float Size_y, float Size_z);		//네모 모양에 콜라이더 생성
	EATER_ENGINEDLL void SetBoxCollider(float Radius);									//네모 콜라이더 생성
	EATER_ENGINEDLL void SetSphereCollider(float Radius);								//원 콜라이더 생성
	EATER_ENGINEDLL void SetCapsuleCollider(float Radius, float Height);				//캡슐 콜라이더 생성
	EATER_ENGINEDLL void SetMeshCollider(std::string MeshName);							//페이스 정보에 따라 콜라이더 생성
	EATER_ENGINEDLL void SetTerrainCollider(std::string MeshName);						//터레인 정보에 따라 콜라이더 생성
	EATER_ENGINEDLL void SetCenter(float x, float y, float z);							//중심점 설정
	EATER_ENGINEDLL void SetTrigger(bool Trigger);			
	//트리거 설정
	EATER_ENGINEDLL void SetMaterial_Static(float Static);					//재질 설정
	EATER_ENGINEDLL void SetMaterial_Dynamic(float Dynamic);				//재질 설정
	EATER_ENGINEDLL void SetMaterial_Restitution(float Restitution);		//재질 설정



	///Get
	EATER_ENGINEDLL PhysCollider* GetCollider();
	EATER_ENGINEDLL bool CreatePhys();

	EATER_ENGINEDLL float GetMaterial_Static();
	EATER_ENGINEDLL float GetMaterial_Dynamic();
	EATER_ENGINEDLL float GetMaterial_Restitution();
private:
	void DebugCollider();
	DirectX::SimpleMath::Matrix CreateXMRot4x4();
private:
	PhysData*		mPhysData;
	PhysCollider*	mColliderData;
	PhysMaterial*	mMaterial;
	Rigidbody*		mRigidbody;
	bool isCreate	= false;

	friend Rigidbody;
};

