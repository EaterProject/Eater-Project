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
	EATER_ENGINEDLL void SetBoxCollider(float Size_x, float Size_y, float Size_z);		//�׸� ��翡 �ݶ��̴� ����
	EATER_ENGINEDLL void SetBoxCollider(float Radius);									//�׸� �ݶ��̴� ����
	EATER_ENGINEDLL void SetSphereCollider(float Radius);								//�� �ݶ��̴� ����
	EATER_ENGINEDLL void SetCapsuleCollider(float Radius, float Height);				//ĸ�� �ݶ��̴� ����
	EATER_ENGINEDLL void SetMeshCollider(std::string MeshName);							//���̽� ������ ���� �ݶ��̴� ����
	EATER_ENGINEDLL void SetTerrainCollider(std::string MeshName);						//�ͷ��� ������ ���� �ݶ��̴� ����
	EATER_ENGINEDLL void SetCenter(float x, float y, float z);							//�߽��� ����
	EATER_ENGINEDLL void SetTrigger(bool Trigger);			
	//Ʈ���� ����
	EATER_ENGINEDLL void SetMaterial_Static(float Static);					//���� ����
	EATER_ENGINEDLL void SetMaterial_Dynamic(float Dynamic);				//���� ����
	EATER_ENGINEDLL void SetMaterial_Restitution(float Restitution);		//���� ����



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

