#include "Phys_Collider_Triangle.h"
#include "BaseData.h"
Phys_Collider_Triangle::Phys_Collider_Triangle()
{

}

Phys_Collider_Triangle::~Phys_Collider_Triangle()
{
	
}

void Phys_Collider_Triangle::Initialize()
{


}

void Phys_Collider_Triangle::Release()
{
	delete[] Pos;
	delete[] index_Array;
}

int Phys_Collider_Triangle::GetVertexCount()
{
	return VertexSize;
}

int Phys_Collider_Triangle::GetIndexCount()
{
	return IndexSize;
}

Phys_Base_Vector3* Phys_Collider_Triangle::GetVertexList()
{
	return Pos;
}

unsigned int* Phys_Collider_Triangle::GetIndexList()
{
	return index_Array;
}

void Phys_Collider_Triangle::SetCreateOption(const int& mVertexSize, const int& mIndexSize)
{
	VertexSize	= mVertexSize;
	IndexSize	= mIndexSize;

	Pos = new Phys_Base_Vector3[mVertexSize];
	index_Array = new unsigned int[IndexSize];
}

void Phys_Collider_Triangle::SetCreateOption(const int&& mVertexSize, const int&& mIndexSize)
{
	VertexSize = mVertexSize;
	IndexSize  = mIndexSize;

	Phys_Base_Vector3* Pos = new Phys_Base_Vector3[mVertexSize];
	index_Array = new unsigned int[IndexSize];
}

void Phys_Collider_Triangle::SetVertexPosition(int& index, float& X, float& Y, float& Z)
{
	Pos[index].x = X;
	Pos[index].y = Y;
	Pos[index].z = Z;
}

void Phys_Collider_Triangle::SetVertexPosition(int&& index,float&& X, float&& Y, float&& Z)
{
	Pos[index].x = X;
	Pos[index].y = Y;
	Pos[index].z = Z;
}

void Phys_Collider_Triangle::SetIndex(int&& index, unsigned int&& mMeshIndex)
{
	index_Array[index] = mMeshIndex;
}

void Phys_Collider_Triangle::SetIndex(int& index,unsigned int& mMeshIndex)
{
	index_Array[index] = mMeshIndex;
}
