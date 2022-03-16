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

	MeshRenderData* m_MeshData;			// ��ȯ�� Mesh Data..
	MaterialRenderData* m_Material;		// ��ȯ�� Material Data..

	TerrainRenderData* m_TerrainData;	// ��ȯ�� Terrain Data..

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
	MeshRenderData* m_Mesh;					// Instancing ������ �Ǵ� Mesh Data..
	MaterialRenderData* m_Material;			// Instancing ������ �Ǵ� Material Data..

	std::vector<RenderData*> m_MeshList;	// Instancing ���ؿ� �ش��ϴ� Render Data List..
};