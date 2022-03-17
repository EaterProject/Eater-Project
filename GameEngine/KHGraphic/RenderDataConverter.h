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

public:
	MeshRenderBuffer* FindMesh(UINT index);
	MaterialRenderBuffer* FindMaterial(UINT index);
	InstanceRenderBuffer* FindInstance(UINT index);

private:
	void ConvertMeshData(MeshBuffer* originBuf, MeshRenderBuffer* convertData);
	void ConvertMaterialData(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);

private:
	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;

	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;
};

