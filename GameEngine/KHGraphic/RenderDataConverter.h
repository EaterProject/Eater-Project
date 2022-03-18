#pragma once
#include <unordered_map>

class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertRenderData(MeshData* originData, RenderData* renderData);
	void ChangeMeshBuffer(MeshBuffer* mesh);
	void ChangeMaterialBuffer(MaterialBuffer* material);

public:
	void PushMesh(MeshBuffer* mesh);
	void PushMaterial(MaterialBuffer* material);
	void PushInstance(MeshRenderBuffer* mesh, MaterialRenderBuffer* material);
	
public:
	void DeleteMesh(UINT index);
	void DeleteMaterial(UINT index);
	void DeleteInstance(UINT index);

public:
	MeshRenderBuffer* FindMesh(UINT index);
	MaterialRenderBuffer* FindMaterial(UINT index);
	InstanceRenderBuffer* FindInstance(MeshRenderBuffer* mesh, MaterialRenderBuffer* material);

private:
	void ConvertMeshData(MeshBuffer* originBuf, MeshRenderBuffer* convertData);
	void ConvertMaterialData(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);

private:

private:
	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;
};

