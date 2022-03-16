#pragma once
#include "ConvertData.h"

class RenderData
{
public:
	RenderData();
	~RenderData();

public:
	void ConvertData(MeshData* originMesh);
	//void ChangeMaterial(MaterialData* )
	void Release();

private:
	void ConvertMeshBuffer(MeshBuffer* originBuf);
	void ConvertMaterial(MaterialData* originMat);
	void ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat);
	
public:
	std::vector<Matrix>* m_BoneOffsetTM;

	MeshData* m_OriginData;				// Engine Origin Data..

	ObjectData* m_ObjectData;

	MeshRenderData* m_MeshData;			// 변환된 Mesh Data..
	MaterialRenderData* m_Material;		// 변환된 Material Data..

	TerrainRenderData* m_TerrainData;	// 변환된 Terrain Data..

	ParticleData* m_ParticleData;
	ColliderData* m_ColliderData;
};

class InstanceRenderData
{
public:
	InstanceRenderData();
	~InstanceRenderData();

public:
	void SetInstanceData(MeshRenderData* mesh, MaterialRenderData* material);

private:
	MeshRenderData* m_Mesh;					// Instancing 기준이 되는 Mesh Data..
	MaterialRenderData* m_Material;			// Instancing 기준이 되는 Material Data..

	std::vector<RenderData*> m_MeshList;	// Instancing 기준에 해당하는 Render Data List..
};