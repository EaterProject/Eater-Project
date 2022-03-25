#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"

class Rigidbody;
class PhysCollider;
class PhysData;
class PhysMaterial;
class TriangleMeshData;
class Transform;

//디버깅용
class ColliderData;

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
	EATER_ENGINEDLL void SetTrigger(bool Trigger);										//트리거 설정
	EATER_ENGINEDLL void SetMaterial(PhysMaterial* m);									//재질 설정
	///Get
	EATER_ENGINEDLL PhysCollider* GetCollider();
private:
	void DebugCollider();
private:
	PhysData*		mPhysData;
	PhysCollider*	mColliderData;
	PhysMaterial*	mMaterial;
	Rigidbody*		mRigidbody;
	Transform*		mTransform;
	ColliderData*	mDebugCollider;

	friend Rigidbody;
};

