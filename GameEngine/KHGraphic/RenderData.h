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
	void ConvertMeshBuffer(MeshBuffer* originBuf);
	void ConvertMaterial(MaterialBuffer* originMat);
	void ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);
	
public:
	std::vector<Matrix>* m_BoneOffsetTM;

	MeshData* m_OriginData;				// Engine Origin Data..

	ObjectData* m_ObjectData;

	MeshRenderBuffer* m_MeshBuffer;			// 변환된 Mesh Buffer..
	MaterialRenderBuffer* m_MaterialBuffer;		// 변환된 Material Buffer..

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
	void SetInstanceData(MeshRenderBuffer* mesh, MaterialRenderBuffer* material);
	void PushInstance(RenderData* renderData);

private:
	MeshRenderBuffer* m_Mesh;					// Instancing 기준이 되는 Mesh Data..
	MaterialRenderBuffer* m_Material;			// Instancing 기준이 되는 Material Data..

	std::vector<RenderData*> m_MeshList;	// Instancing 기준에 해당하는 Render Data List..
};