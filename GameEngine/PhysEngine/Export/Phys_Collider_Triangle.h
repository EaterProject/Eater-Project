#pragma once
#include "Phys_Collider_Interface.h"

struct Phys_Base_Vector3;
class Phys_Collider_Triangle : public Phys_Collider_Interface
{
public:
	Phys_Collider_Triangle();
	virtual ~Phys_Collider_Triangle();

	PHYS_ENGINEDLL void Initialize() override;
	PHYS_ENGINEDLL void Release() override;

	PHYS_ENGINEDLL int GetVertexCount();
	PHYS_ENGINEDLL int GetIndexCount();

	PHYS_ENGINEDLL Phys_Base_Vector3* GetVertexList();
	PHYS_ENGINEDLL unsigned int* GetIndexList();

	PHYS_ENGINEDLL void SetCreateOption(const int& mVertexSize, const int& mIndexSize);
	PHYS_ENGINEDLL void SetCreateOption(const int&& mVertexSize, const int&& mIndexSize);

	PHYS_ENGINEDLL void SetVertexPosition(int& index, float& X, float& Y, float& Z);
	PHYS_ENGINEDLL void SetVertexPosition(int&& index, float&& X, float&& Y, float&& Z);

	PHYS_ENGINEDLL void SetIndex(int&& index, unsigned int&& mMeshIndex);
	PHYS_ENGINEDLL void SetIndex(int& index, unsigned int& mMeshIndex);
private:
	Phys_Base_Vector3* Pos		= nullptr;
	unsigned int* index_Array	= nullptr;

	int VertexSize	= 0;
	int IndexSize	= 0;
};

