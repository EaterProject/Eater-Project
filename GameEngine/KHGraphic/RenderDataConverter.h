#pragma once
#include <unordered_map>

/// 2022/03/19 20:00
/// SeoKyuHwang
///
/// # RenderDataConverter Class
///
/// - Game Engine 측에서 받은 Mesh Data를 Renderer 구조에 1:1 대응하는 구조체로 미리 캐스팅..
/// - 해당 Mesh Buffer 와 Material Buffer는 Game Engine Reource Load Time에 등록해둔다..
/// - 추가적으로 들어온 Object의 Mesh Data에선 미리 리스트업 된 Render Mesh Buffer와 Render Material Buffer

class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	RenderData* ConvertRenderData(MeshData* originData);
	void ResourceUpdate();

	void Release();

public:
	void PushMesh(MeshBuffer* mesh);
	void PushMaterial(MaterialBuffer* material);
	void PushChangeMesh(MeshBuffer* mesh);
	void PushChangeMaterial(MaterialBuffer* material);

public:
	void DeleteInstance(UINT index);
	void DeleteMesh(UINT index);
	void DeleteMaterial(UINT index);

public:
	MeshRenderBuffer* GetMesh(UINT index);
	MaterialRenderBuffer* GetMaterial(UINT index);
	InstanceRenderBuffer* GetInstance(UINT index);
	InstanceLayer* GetLayer(UINT index);

private:
	void ConvertPushResource();
	void ConvertChangeResource();

	void ConvertPushMesh(MeshBuffer* mesh);
	void ConvertPushMaterial(MaterialBuffer* material);

	void ConvertChangeMesh(MeshBuffer* mesh);
	void ConvertChangeMaterial(MaterialBuffer* material);

	void ConvertMesh(MeshBuffer* originBuf, MeshRenderBuffer* convertData);
	void ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);

	InstanceRenderBuffer* FindInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material);
	InstanceRenderBuffer* CheckInstance(MeshRenderBuffer* mesh, MaterialRenderBuffer* material);
	void CheckEmptyInstance(MeshRenderBuffer* mesh);
	void CheckEmptyInstance(MaterialRenderBuffer* material);

private:
	std::queue<MeshBuffer*> m_PushMeshList;
	std::queue<MeshBuffer*> m_ChangeMeshList;
	std::queue<MaterialBuffer*> m_PushMaterialList;
	std::queue<MaterialBuffer*> m_ChangeMaterialList;

	std::unordered_map<UINT, RenderData*> m_RenderDataList;
	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;

	std::vector<std::pair<UINT, bool>> m_LayerIndexList;
	std::unordered_map<UINT, TerrainRenderBuffer*> m_LayerList;

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;

	std::vector<std::pair<UINT, bool>> m_InstanceLayerIndexList;
	std::unordered_map<UINT, InstanceLayer*> m_InstanceLayerList;
};

