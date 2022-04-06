#pragma once
#include <unordered_map>

/// 2022/03/19 20:00
/// SeoKyuHwang
///
/// # RenderDataConverter Class
///
/// - Game Engine ������ ���� Mesh Data�� Renderer ������ 1:1 �����ϴ� ����ü�� �̸� ĳ����..
/// - �ش� Mesh Buffer �� Material Buffer�� Game Engine Reource Load Time�� ����صд�..
/// - �߰������� ���� Object�� Mesh Data���� �̸� ����Ʈ�� �� Render Mesh Buffer�� Render Material Buffer
/// 
/// - Render Resource ��� �� ����ȭ ����
///  1) ���� Frame�� �߰��ǰų� ����� Resource ������ Queue�� �����صд�.
///  2) ���� -> ���� ������ Resource Update �ϹǷ� ������ Resource�� �����ϴ��� ���� ���� ����.
///  3) Renderer ���� ȣ��Ǳ� ���� ���� Frame�� Update�� ��ģ �����̹Ƿ�
///     ��� �κп� �߰��� �ϴ� Update �� ó���� �Ͽ� Rendering�� �� ���̹Ƿ� �������� ����.
///  4) �������� Resource Instance Layer ������ �� Ŭ�������� �ϰ� 
///     �ش� Layer�� �����Ͽ� Render Manager���� Rendering�� �Ѵ�.

class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertMeshData(MeshData* originData, RenderData* renderData);		// Mesh Data -> Render Data ��ȯ..
	void ConvertRenderData(MeshData* originData, RenderData* renderData);	// Mesh Data -> Render Data ��ȯ..
	
	void ResourceUpdate();		// ���� �����ӿ� ���� & ���� Resource Update..

	void Release();

public:
	void PushMesh(MeshBuffer* mesh);					// ���� �����ӿ� ������ Mesh �ӽ� Queue�� ����..
	void PushMaterial(MaterialBuffer* material);		// ���� �����ӿ� ������ Material �ӽ� Queue�� ����..
	void PushChangeMesh(MeshBuffer* mesh);				// ���� �����ӿ� ����� Mesh �ӽ� Queue�� ����..
	void PushChangeMaterial(MaterialBuffer* material);  // ���� �����ӿ� ����� Material �ӽ� Queue�� ����..

public:
	void DeleteRenderData(UINT index);		// �ش� Render Data ��� ����..
	void DeleteInstance(UINT index);		// �ش� Instance Resource ��� ����..
	void DeleteMesh(UINT index);			// �ش� Mesh Resource ��� ����..
	void DeleteMaterial(UINT index);		// �ش� Material Resource ��� ����..

public:
	RenderData* GetRenderData(UINT index);			// Render Data ���� Index�� �˻�..
	MeshRenderBuffer* GetMesh(UINT index);			// Mesh Resource ���� Index�� �˻�..
	MaterialRenderBuffer* GetMaterial(UINT index);	// Material Resource ���� Index�� �˻�..
	InstanceRenderBuffer* GetInstance(UINT index);	// Instance Resource ���� Index�� �˻�..
	InstanceLayer* GetLayer(UINT index);			// Layer Resource ���� Index�� �˻�..

private:
	void ConvertPushResource();			// ���� �����ӿ� ������ Resource Update..
	void ConvertChangeResource();		// ���� �����ӿ� ����� Resource Update..

	void ConvertPushMesh(MeshBuffer* mesh);					// ������ Mesh Resource ��ȯ �� ���..
	void ConvertPushMaterial(MaterialBuffer* material);		// ������ Material Resource ��ȯ �� ���..

	void ConvertChangeMesh(MeshBuffer* mesh);				// ����� Mesh Resource ��ȯ..
	void ConvertChangeMaterial(MaterialBuffer* material);	// ����� Material Resource ��ȯ..

	void ConvertMesh(MeshBuffer* originBuf, MeshRenderBuffer* convertData);				// ������ Mesh Resource ��ȯ..
	void ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);	// ������ Material Resource ��ȯ..

	void RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material);		// Mesh & Material Resource ���� Instance �˻� �� ���..
	
	void CheckEmptyInstance(MeshRenderBuffer* mesh);			// Mesh Resource ������ ���õ� Instance ����..
	void CheckEmptyInstance(MaterialRenderBuffer* material);	// Material Resource ������ ���õ� Instance ����..

private:
	std::queue<MeshBuffer*> m_PushMeshList;				// Game Engine ������ ���� �����ӿ� ������ Mesh Resource Queue..
	std::queue<MeshBuffer*> m_ChangeMeshList;			// Game Engine ������ ���� �����ӿ� ������ Mesh Resource Queue..
	std::queue<MaterialBuffer*> m_PushMaterialList;		// Game Engine ������ ���� �����ӿ� ������ Material Resource Queue..
	std::queue<MaterialBuffer*> m_ChangeMaterialList;	// Game Engine ������ ���� �����ӿ� ������ Material Resource Queue..

	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;			// ���� ��� �Ǿ��ִ� Mesh Resource..
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;	// ���� ��� �Ǿ��ִ� Material Resource..
	std::unordered_map<UINT, TerrainRenderBuffer*> m_LayerList;		// ���� ��� �Ǿ��ִ� Terrain Resource..

	std::unordered_map<UINT, RenderData*> m_RenderList;				// ���� ��� �Ǿ��ִ� ��� Render Data..

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;			// Instance Index ������ ���� List..
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;	// ���� ��� �Ǿ��ִ� Instance Resource..
	std::unordered_map<UINT, InstanceLayer*> m_InstanceLayerList;	// Instance Resource�� �����ϴ� Mesh List Layer..
};

