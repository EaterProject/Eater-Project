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
	//�׸� �ݶ��̴�
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
	//�ݶ��̴� ������ ����
	Vector3 Size;						//������
	Vector3 Center;						//�ݶ��̴��� ���۵Ǵ� ��ġ �߽���
	PhysCollider::TYPE	Shape_type;		//Ÿ��
	TriangleMeshData* TriangleData;		//���̽� �ݶ��̴� ������
	bool isTrigger;						//Ʈ���� ����
};

