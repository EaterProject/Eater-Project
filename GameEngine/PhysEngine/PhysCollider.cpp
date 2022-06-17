#include "PhysCollider.h"

#include "Phys_Collider_Box.h"
#include "Phys_Collider_Capsule.h"
#include "Phys_Collider_Sphere.h"
#include "Phys_Collider_Triangle.h"

PhysCollider::PhysCollider()
{
	Size			= { 1.0f,1.0f,1.0f };
	Center			= { 0.0f,0.0f,0.0f };
	Shape_type		= TYPE::BOX;
	isTrigger		= false;
}

PhysCollider::~PhysCollider()
{
	if (Shape_type == TYPE::TERRAIN || Shape_type == TYPE::MESH) 
	{
		
	}
}

void PhysCollider::SetBoxCollider(float Size_x, float Size_y, float Size_z)
{
	Collider_box = CreateBox(Size_x, Size_y, Size_z);
	Shape_type = PhysCollider::TYPE::BOX;
}

void PhysCollider::SetBoxCollider(float Radius)
{
	Collider_box = CreateBox(Radius, Radius, Radius);
	Shape_type = PhysCollider::TYPE::BOX;
}

void PhysCollider::SetSphereCollider(float Radius)
{
	Collider_Sphere = CreateSphere(Radius);
	Shape_type = PhysCollider::TYPE::SPHERE;
}

void PhysCollider::SetCapsuleCollider(float Radius, float Height)
{
	Size = { Radius, Height,0.0f};
	Shape_type = PhysCollider::TYPE::CAPSULE;
}

void PhysCollider::SetCenter(float x, float y, float z)
{
	Center = { x,y,z };
}

void PhysCollider::SetTrigger(bool Trigger)
{
	isTrigger = Trigger;
}

Phys_Collider_Triangle* PhysCollider::CreateTriangle(int indexSize, int VertexSize, UINT* IndexArray, Vector3* VertexList)
{
	//콜라이더 생성
	Collider_Triangle	= new Phys_Collider_Triangle();
	Collider_Triangle->SetCreateOption(VertexSize,indexSize);

	Shape_type			= PhysCollider::TYPE::MESH;

	for (int i = 0; i < VertexSize; i++)
	{
		Collider_Triangle->SetVertexPosition(i, VertexList[i].x, VertexList[i].y, VertexList[i].z);
	}

	for (int i = 0; i < indexSize; i++)
	{
		Collider_Triangle->SetIndex(i, IndexArray[i]);
	}

	return Collider_Triangle;
}

Phys_Collider_Sphere* PhysCollider::CreateSphere(float Radius)
{	
	Collider_Sphere = new Phys_Collider_Sphere();
	Collider_Sphere->Initialize();
	Collider_Sphere->SetSize(Radius);
	return Collider_Sphere;
}

Phys_Collider_Box* PhysCollider::CreateBox(float Size_x, float Size_y, float Size_z)
{
	Collider_box = new Phys_Collider_Box();
	Collider_box->Initialize();
	Collider_box->SetSize(Size_x, Size_y, Size_z);
	return Collider_box;
}

Phys_Collider_Capsule* PhysCollider::CreateCapsule(float Radius, float Height)
{
	return nullptr;
}

Vector3 PhysCollider::GetCenter()
{
	return Center;
}

Vector3 PhysCollider::GetSize()
{
	return Size;
}

PhysCollider::TYPE PhysCollider::GetType()
{
	return Shape_type;
}

bool PhysCollider::GetTrigger()
{
	return isTrigger;
}

 Phys_Collider_Box* PhysCollider::GetBoxCollider()
 {
	 return Collider_box;
 }

 Phys_Collider_Sphere* PhysCollider::GetSphereCollider()
 {
	 return Collider_Sphere;
 }

 Phys_Collider_Triangle* PhysCollider::GetTriangleCollider()
 {
	 return Collider_Triangle;
 }

 Phys_Collider_Capsule* PhysCollider::GetCapsuleCollider()
 {
	 return Collider_Capsule;
 }

