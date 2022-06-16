#pragma once

#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <string>

using namespace DirectX;
using namespace SimpleMath;

///�ݶ��̴� ������
///Center�� �߽����� ���� ���� Shape_Size ��ŭ �ݶ��̴��� �������ش�
///�⺻������ BoxCollider �� �� ���� 1������� �����Ѵ�
/// 
class Phys_Collider_Box;
class Phys_Collider_Sphere;
class Phys_Collider_Capsule;
class Phys_Collider_Triangle;

class  PhysCollider
{
public:
	PHYS_ENGINEDLL  PhysCollider();
	PHYS_ENGINEDLL ~PhysCollider();

	enum class TYPE
	{
		BOX,
		SPHERE,
		CAPSULE,
		MESH,
		TERRAIN,
	};

public:
	///Create
	PHYS_ENGINEDLL Phys_Collider_Triangle*	CreateTriangle(int indexSize, int VertexSize,UINT* IndexArray,Vector3* VertexList);
	PHYS_ENGINEDLL Phys_Collider_Sphere*	CreateSphere(float Radius);
	PHYS_ENGINEDLL Phys_Collider_Box*		CreateBox(float Size_x, float Size_y, float Size_z);
	PHYS_ENGINEDLL Phys_Collider_Capsule*	CreateCapsule(float Radius, float Height);
public:
	///Set
	PHYS_ENGINEDLL void SetBoxCollider(float Size_x,float Size_y,float Size_z);
	PHYS_ENGINEDLL void SetBoxCollider(float Radius);

	PHYS_ENGINEDLL void SetSphereCollider(float Radius);

	PHYS_ENGINEDLL void SetCapsuleCollider(float Radius, float Height);


	PHYS_ENGINEDLL void SetCenter(float x, float y, float z);
	PHYS_ENGINEDLL void SetTrigger(bool Trigger);
public:
	///Get
	PHYS_ENGINEDLL Vector3 GetCenter();
	PHYS_ENGINEDLL Vector3 GetSize();
	PHYS_ENGINEDLL TYPE GetType();
	PHYS_ENGINEDLL bool GetTrigger();
public:
	Phys_Collider_Box*		GetBoxCollider();
	Phys_Collider_Sphere*	GetSphereCollider();
	Phys_Collider_Triangle* GetTriangleCollider();
	Phys_Collider_Capsule*	GetCapsuleCollider();
private:
	//�ݶ��̴� ������ ����
	Vector3 Size;						//������
	Vector3 Center;						//�ݶ��̴��� ���۵Ǵ� ��ġ �߽���

	Phys_Collider_Box*		Collider_box		= nullptr;
	Phys_Collider_Sphere*	Collider_Sphere		= nullptr;
	Phys_Collider_Triangle* Collider_Triangle	= nullptr;
	Phys_Collider_Capsule*	Collider_Capsule	= nullptr;

	PhysCollider::TYPE	Shape_type;		//Ÿ��
	bool isTrigger;						//Ʈ���� ����
};

