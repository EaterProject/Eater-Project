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

	MeshRenderBuffer* m_MeshBuffer;			// ��ȯ�� Mesh Buffer..
	MaterialRenderBuffer* m_MaterialBuffer;		// ��ȯ�� Material Buffer..

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
	void SetInstanceData(MeshRenderBuffer* mesh, MaterialRenderBuffer* material);
	void PushInstance(RenderData* renderData);

private:
	MeshRenderBuffer* m_Mesh;					// Instancing ������ �Ǵ� Mesh Data..
	MaterialRenderBuffer* m_Material;			// Instancing ������ �Ǵ� Material Data..

	std::vector<RenderData*> m_MeshList;	// Instancing ���ؿ� �ش��ϴ� Render Data List..
};