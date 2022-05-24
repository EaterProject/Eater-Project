#include "PhysCollider.h"

PhysCollider::PhysCollider()
{
	Size			= { 1.0f,1.0f,1.0f };
	Center			= { 0.0f,0.0f,0.0f };
	Shape_type		= TYPE::BOX;
	isTrigger		= false;
	TriangleData	= nullptr;
	TriangleData	= nullptr;
}

PhysCollider::~PhysCollider()
{
	if (Shape_type == TYPE::TERRAIN || Shape_type == TYPE::MESH) 
	{
		if (TriangleData != nullptr)
		{
			delete TriangleData;
		}
	}
}

TriangleMeshData::TriangleMeshData()
{
	Name			= "";
	VertexList		= nullptr;
	CIndexList		= nullptr;
	VertexListSize	= 0;
	IndexListSize	= 0;
}

TriangleMeshData::~TriangleMeshData()
{
	if (VertexListSize != 0)
	{
		delete[] VertexList;
	}

	if (IndexListSize != 0)
	{
		delete[] CIndexList;
	}
}

void PhysCollider::SetBoxCollider(float Size_x, float Size_y, float Size_z)
{
	Size = { Size_x,Size_y,Size_z };
	Shape_type = PhysCollider::TYPE::BOX;
}

void PhysCollider::SetBoxCollider(float Radius)
{
	Size = { Radius,Radius,Radius };
	Shape_type = PhysCollider::TYPE::BOX;
}

void PhysCollider::SetSphereCollider(float Radius)
{
	Size = { Radius,Radius,Radius };
	Shape_type = PhysCollider::TYPE::SPHERE;
}

void PhysCollider::SetCapsuleCollider(float Radius, float Height)
{
	Size = { Radius, Height,0.0f};
	Shape_type = PhysCollider::TYPE::CAPSULE;
}

void PhysCollider::SetMeshCollider(TriangleMeshData* mMeshData)
{
	TriangleData = mMeshData;
	Shape_type	= PhysCollider::TYPE::MESH;
}

void PhysCollider::SetTerrainCollider(TriangleMeshData* mMeshData, float Height)
{
	TriangleData = mMeshData;
	Shape_type = PhysCollider::TYPE::TERRAIN;
}

void PhysCollider::SetCenter(float x, float y, float z)
{
	Center = { x,y,z };
}

void PhysCollider::SetTrigger(bool Trigger)
{
	isTrigger = Trigger;
}

TriangleMeshData* PhysCollider::CreateTriangle(int indexSize, int VertexSize, UINT* IndexArray, Vector3* VertexList)
{
	TriangleData = new TriangleMeshData();
	Shape_type = PhysCollider::TYPE::MESH;
	TriangleData->VertexList = new Vector3[VertexSize];
	TriangleData->CIndexList = new UINT[indexSize];
	TriangleData->IndexListSize		= indexSize;
	TriangleData->VertexListSize	= VertexSize;

	for (int i = 0; i < indexSize; i++)
	{
		TriangleData->CIndexList[i] = IndexArray[i];
	}

	for (int i = 0; i < VertexSize; i++)
	{
		TriangleData->VertexList[i] = VertexList[i];
	}


	return TriangleData;
}

TriangleMeshData* PhysCollider::CreateTerrain()
{
	TriangleData = new TriangleMeshData();
	Shape_type = PhysCollider::TYPE::TERRAIN;
	return TriangleData;
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

TriangleMeshData* PhysCollider::GetTriangleMesh()
{
	return TriangleData;
}

 std::string PhysCollider::GetTriangleMeshName()
{
	 return TriangleData->Name;
}

