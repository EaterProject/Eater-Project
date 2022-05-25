#pragma once
#include <unordered_map>
#include "RenderDataConverterBase.h"

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

class RenderDataConverter : public IRenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertMeshData(MeshData* originData, RenderData* renderData) override;	// Mesh Data -> Render Data ��ȯ..
	void ConvertRenderData(MeshData* originData, RenderData* renderData) override;	// Mesh Data -> Render Data ��ȯ..
	
	void ResourceUpdate() override;		// ���� �����ӿ� ���� & ���� Resource Update..

	void Release() override;

public:
	void PushMesh(MeshBuffer* mesh) override;						// ���� �����ӿ� ������ Mesh �ӽ� Queue�� ����..
	void PushMaterial(MaterialBuffer* material) override;			// ���� �����ӿ� ������ Material �ӽ� Queue�� ����..
	void PushAnimation(AnimationBuffer* animation) override;		// ���� �����ӿ� ������ Animation �ӽ� Queue�� ����..

public:
	void PushChangeMesh(MeshBuffer* mesh) override;					// ���� �����ӿ� ����� Mesh �ӽ� Queue�� ����..
	void PushChangeMaterial(MaterialBuffer* material) override;		// ���� �����ӿ� ����� Material �ӽ� Queue�� ����..
	void PushChangeAnimation(AnimationBuffer* animation) override;	// ���� �����ӿ� ������ Animation �ӽ� Queue�� ����..

public:
	void DeleteRenderData(UINT index) override;						// �ش� Render Data ��� ����..
	void DeleteInstance(UINT index) override;						// �ش� Instance Resource ��� ����..
	void DeleteMesh(UINT index) override;							// �ش� Mesh Resource ��� ����..
	void DeleteMaterial(UINT index) override;						// �ش� Material Resource ��� ����..
	void DeleteAnimation(UINT index) override;						// �ش� Animation Resource ��� ����..
	void DeleteUI(UINT index) override;								// �ش� UI Resource ��� ����..

public:
	size_t FindMaxInstanceCount() override;							// ��� Layer ������ Instance ���� �� ���� ū ���� ��ȯ..

public:
	RenderData* GetRenderData(int index) override;					// Render Data ���� Index�� �˻�..
	MeshRenderBuffer* GetMesh(int index) override;					// Mesh Resource ���� Index�� �˻�..
	MaterialRenderBuffer* GetMaterial(int index) override;			// Material Resource ���� Index�� �˻�..
	AnimationRenderBuffer* GetAnimation(int index) override;		// Animation Resource ���� Index�� �˻�..
	InstanceRenderBuffer* GetInstance(int index) override;			// Instance Resource ���� Index�� �˻�..
	InstanceLayer* GetLayer(int index) override;					// Layer Resource ���� Index�� �˻�..

private:
	void ConvertPushResource();			// ���� �����ӿ� ������ Resource Update..
	void ConvertChangeResource();		// ���� �����ӿ� ����� Resource Update..

	template<typename Origin, typename Convert>
	void ConvertPushResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList);

	template<typename Origin, typename Convert>
	void ConvertChangeResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList);

	template<typename Origin, typename Convert>
	void ConvertResource(Origin* origin, Convert* convert);

	void RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material, AnimationRenderBuffer* animation);		// Mesh & Material Resource ���� Instance �˻� �� ���..
	
	void CheckEmptyInstance(MeshRenderBuffer* mesh);			// Mesh Resource ������ ���õ� Instance ����..
	void CheckEmptyInstance(MaterialRenderBuffer* material);	// Material Resource ������ ���õ� Instance ����..
	void CheckEmptyInstance(AnimationRenderBuffer* animation);	// Animation Resource ������ ���õ� Instance ����..

private:
	std::queue<MeshBuffer*>			m_PushMeshList;							// Game Engine ������ ���� �����ӿ� ������ Mesh Resource Queue..
	std::queue<MeshBuffer*>			m_ChangeMeshList;						// Game Engine ������ ���� �����ӿ� ������ Mesh Resource Queue..
	std::queue<MaterialBuffer*>		m_PushMaterialList;						// Game Engine ������ ���� �����ӿ� ������ Material Resource Queue..
	std::queue<MaterialBuffer*>		m_ChangeMaterialList;					// Game Engine ������ ���� �����ӿ� ������ Material Resource Queue..
	std::queue<AnimationBuffer*>	m_PushAnimationList;					// Game Engine ������ ���� �����ӿ� ������ Animation Resource Queue..
	std::queue<AnimationBuffer*>	m_ChangeAnimationList;					// Game Engine ������ ���� �����ӿ� ������ Animation Resource Queue..

	std::unordered_map<UINT, MeshRenderBuffer*>			m_MeshList;			// ���� ��� �Ǿ��ִ� Mesh Resource..
	std::unordered_map<UINT, MaterialRenderBuffer*>		m_MaterialList;		// ���� ��� �Ǿ��ִ� Material Resource..
	std::unordered_map<UINT, TerrainRenderBuffer*>		m_TerrainList;		// ���� ��� �Ǿ��ִ� Terrain Resource..
	std::unordered_map<UINT, UIRenderBuffer*>			m_UIList;			// ���� ��� �Ǿ��ִ� UI Resource..
	std::unordered_map<UINT, AnimationRenderBuffer*>	m_AnimationList;	// ���� ��� �Ǿ��ִ� Animation Resource..

	std::unordered_map<UINT, RenderData*> m_RenderList;				// ���� ��� �Ǿ��ִ� ��� Render Data..

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;			// Instance Index ������ ���� List..
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;	// ���� ��� �Ǿ��ִ� Instance Resource..
	std::unordered_map<UINT, InstanceLayer*> m_InstanceLayerList;	// Instance Resource�� �����ϴ� Mesh List Layer..
};

template<typename Origin, typename Convert>
inline void RenderDataConverter::ConvertPushResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList)
{
	// Material Index..
	UINT resourceIndex = origin->BufferIndex;

	// �ش� Index Resource üũ..
	auto itor = convertList.find(resourceIndex);
	assert(itor == convertList.end());

	// ���ο� Resource Render Buffer ����..
	Convert* newResource = new Convert();

	// Resource Render Buffer ��ȯ..
	ConvertResource<Origin, Convert>(origin, newResource);

	// Resource Render Buffer ����..
	convertList.insert(std::pair<UINT, Convert*>(resourceIndex, newResource));
}

template<typename Origin, typename Convert>
inline void RenderDataConverter::ConvertChangeResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList)
{
	// Resource Index..
	UINT resourceIndex = origin->BufferIndex;

	// �ش� Index Resource üũ..
	auto itor = convertList.find(resourceIndex);
	assert(itor != convertList.end());

	// Resource Render Buffer �缳��..
	ConvertResource<Origin, Convert>(origin, itor->second);
}

template<typename Origin, typename Convert>
inline void RenderDataConverter::ConvertResource(Origin* origin, Convert* convert) {}

template<>
inline void RenderDataConverter::ConvertResource(MeshBuffer* origin, MeshRenderBuffer* convert);

template<>
inline void RenderDataConverter::ConvertResource(MaterialBuffer* origin, MaterialRenderBuffer* convert);

template<>
inline void RenderDataConverter::ConvertResource(AnimationBuffer* origin, AnimationRenderBuffer* convert);
