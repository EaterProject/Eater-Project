#pragma once

#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <string>

using namespace DirectX;
using namespace SimpleMath;

///콜라이더 정보들
///Center를 중심점을 시작 으로 Shape_Size 만큼 콜라이더를 생성해준다
///기본값으로 BoxCollider 와 각 축의 1사이즈로 생성한다
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
	//콜라이더 데이터 셋팅
	Vector3 Size;						//사이즈
	Vector3 Center;						//콜라이더가 시작되는 위치 중심점

	Phys_Collider_Box*		Collider_box		= nullptr;
	Phys_Collider_Sphere*	Collider_Sphere		= nullptr;
	Phys_Collider_Triangle* Collider_Triangle	= nullptr;
	Phys_Collider_Capsule*	Collider_Capsule	= nullptr;

	PhysCollider::TYPE	Shape_type;		//타입
	bool isTrigger;						//트리거 여부
};

