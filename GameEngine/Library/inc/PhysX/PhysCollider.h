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

class PHYS_ENGINEDLL TriangleMeshData
{
public:
	TriangleMeshData();
	~TriangleMeshData();
public:
	std::string Name;
	Vector3* VertexList = nullptr;
	UINT* CIndexList = nullptr;
	int VertexListSize = 0;
	int IndexListSize = 0;
};

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
	PHYS_ENGINEDLL TriangleMeshData* CreateTriangle(int indexSize, int VertexSize,UINT* IndexArray,Vector3* VertexList);
	PHYS_ENGINEDLL TriangleMeshData* CreateTerrain();
public:
	///Set
	//네모 콜라이더
	PHYS_ENGINEDLL void SetBoxCollider(float Size_x,float Size_y,float Size_z);
	PHYS_ENGINEDLL void SetBoxCollider(float Radius);
	PHYS_ENGINEDLL void SetSphereCollider(float Radius);
	PHYS_ENGINEDLL void SetCapsuleCollider(float Radius, float Height);
	PHYS_ENGINEDLL void SetMeshCollider(TriangleMeshData* MeshData);
	PHYS_ENGINEDLL void SetTerrainCollider(TriangleMeshData* MeshData, float Height);
	PHYS_ENGINEDLL void SetCenter(float x, float y, float z);
	PHYS_ENGINEDLL void SetTrigger(bool Trigger);
public:
	///Get
	PHYS_ENGINEDLL Vector3 GetCenter();
	PHYS_ENGINEDLL Vector3 GetSize();
	PHYS_ENGINEDLL TYPE GetType();
	PHYS_ENGINEDLL bool GetTrigger();
	PHYS_ENGINEDLL TriangleMeshData* GetTriangleMesh();
	PHYS_ENGINEDLL std::string GetTriangleMeshName();
private:
	//콜라이더 데이터 셋팅
	Vector3 Size;						//사이즈
	Vector3 Center;						//콜라이더가 시작되는 위치 중심점
	PhysCollider::TYPE	Shape_type;		//타입
	TriangleMeshData* TriangleData;		//페이스 콜라이더 데이터
	bool isTrigger;						//트리거 여부
};

