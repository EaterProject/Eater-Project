#pragma once
#include <unordered_map>

class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertRenderData(MeshData* originData, RenderData* renderData);
	void ChangeMeshData(UINT index);
	void ChangeMaterialData(UINT index);

public:
	MeshRenderBuffer* GetMeshRenderData(UINT index);
	MaterialRenderBuffer* GetMaterialRenderData(UINT index);

private:
	void ConvertMeshBuffer(MeshBuffer* originBuf, MeshRenderBuffer* convertData);
	void ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);

private:
	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;
};

