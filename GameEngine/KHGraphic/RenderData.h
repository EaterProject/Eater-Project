#pragma once
#include "ConvertData.h"

class RenderData
{
public:
	RenderData();
	~RenderData();

public:
	void ConvertData(MeshData* originMesh);

	void Release();

private:
	void ConvertIndexBuffer(IndexBuffer* originBuf);
	void ConvertVertexBuffer(VertexBuffer* originBuf);
	void ConvertMaterial(MaterialData* originMat);
	void ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat);
	
public:
	OBJECT_TYPE m_ObjectType;

	UINT m_MeshIndex;

	Matrix* m_World;
	std::vector<Matrix>* m_BoneOffsetTM;

	MeshData* m_OriginData;				// Engine Origin Data..

	MeshRenderData* m_MeshData;			// ��ȯ�� Mesh Data..
	TerrainRenderData* m_TerrainData;	// ��ȯ�� Terrain Data..

	ParticleData* m_ParticleData;
	ColliderData* m_ColliderData;
};