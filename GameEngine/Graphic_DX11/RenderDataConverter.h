#pragma once
#include <unordered_map>
#include "RenderDataConverterBase.h"

/// 2022/03/19 20:00
/// SeoKyuHwang
///
/// # RenderDataConverter Class
///
/// - Game Engine 측에서 받은 Mesh Data를 Renderer 구조에 1:1 대응하는 구조체로 미리 캐스팅..
/// - 해당 Mesh Buffer 와 Material Buffer는 Game Engine Reource Load Time에 등록해둔다..
/// - 추가적으로 들어온 Object의 Mesh Data에선 미리 리스트업 된 Render Mesh Buffer와 Render Material Buffer
/// 
/// - Render Resource 등록 및 동기화 순서
///  1) 현재 Frame에 추가되거나 변경된 Resource 각각의 Queue에 삽입해둔다.
///  2) 생성 -> 변경 순으로 Resource Update 하므로 생성된 Resource를 변경하더라도 순서 보장 가능.
///  3) Renderer 측에 호출되기 전에 현재 Frame의 Update는 마친 상태이므로
///     어느 부분에 추가를 하던 Update 후 처리를 하여 Rendering을 할 것이므로 순서보장 가능.
///  4) 실질적인 Resource Instance Layer 관리는 위 클래스에서 하고 
///     해당 Layer를 참조하여 Render Manager에서 Rendering만 한다.

class RenderDataConverter : public IRenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertMeshData(MeshData* originData, RenderData* renderData) override;	// Mesh Data -> Render Data 변환..
	void ConvertRenderData(MeshData* originData, RenderData* renderData) override;	// Mesh Data -> Render Data 변환..
	
	void ResourceUpdate() override;		// 현재 프레임에 생성 & 삽입 Resource Update..

	void Release() override;

public:
	void PushMesh(MeshBuffer* mesh) override;						// 현재 프레임에 생성된 Mesh 임시 Queue에 삽입..
	void PushMaterial(MaterialBuffer* material) override;			// 현재 프레임에 생성된 Material 임시 Queue에 삽입..
	void PushAnimation(AnimationBuffer* animation) override;		// 현재 프레임에 생성된 Animation 임시 Queue에 삽입..

public:
	void PushChangeMesh(MeshBuffer* mesh) override;					// 현재 프레임에 변경된 Mesh 임시 Queue에 삽입..
	void PushChangeMaterial(MaterialBuffer* material) override;		// 현재 프레임에 변경된 Material 임시 Queue에 삽입..
	void PushChangeAnimation(AnimationBuffer* animation) override;	// 현재 프레임에 생성된 Animation 임시 Queue에 삽입..

public:
	void DeleteRenderData(UINT index) override;						// 해당 Render Data 즉시 제거..
	void DeleteInstance(UINT index) override;						// 해당 Instance Resource 즉시 제거..
	void DeleteMesh(UINT index) override;							// 해당 Mesh Resource 즉시 제거..
	void DeleteMaterial(UINT index) override;						// 해당 Material Resource 즉시 제거..
	void DeleteAnimation(UINT index) override;						// 해당 Animation Resource 즉시 제거..
	void DeleteUI(UINT index) override;								// 해당 UI Resource 즉시 제거..

public:
	size_t FindMaxInstanceCount() override;							// 모든 Layer 내부의 Instance 개수 중 제일 큰 개수 반환..

public:
	RenderData* GetRenderData(int index) override;					// Render Data 고유 Index로 검색..
	MeshRenderBuffer* GetMesh(int index) override;					// Mesh Resource 고유 Index로 검색..
	MaterialRenderBuffer* GetMaterial(int index) override;			// Material Resource 고유 Index로 검색..
	AnimationRenderBuffer* GetAnimation(int index) override;		// Animation Resource 고유 Index로 검색..
	InstanceRenderBuffer* GetInstance(int index) override;			// Instance Resource 고유 Index로 검색..
	InstanceLayer* GetLayer(int index) override;					// Layer Resource 고유 Index로 검색..

private:
	void ConvertPushResource();			// 현재 프레임에 생성된 Resource Update..
	void ConvertChangeResource();		// 현재 프레임에 변경된 Resource Update..

	template<typename Origin, typename Convert>
	void ConvertPushResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList);

	template<typename Origin, typename Convert>
	void ConvertChangeResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList);

	template<typename Origin, typename Convert>
	void ConvertResource(Origin* origin, Convert* convert);

	void RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material, AnimationRenderBuffer* animation);		// Mesh & Material Resource 기준 Instance 검색 및 등록..
	
	void CheckEmptyInstance(MeshRenderBuffer* mesh);			// Mesh Resource 삭제시 관련된 Instance 삭제..
	void CheckEmptyInstance(MaterialRenderBuffer* material);	// Material Resource 삭제시 관련된 Instance 삭제..
	void CheckEmptyInstance(AnimationRenderBuffer* animation);	// Animation Resource 삭제시 관련된 Instance 삭제..

private:
	std::queue<MeshBuffer*>			m_PushMeshList;							// Game Engine 측에서 현재 프레임에 생성한 Mesh Resource Queue..
	std::queue<MeshBuffer*>			m_ChangeMeshList;						// Game Engine 측에서 현재 프레임에 변경한 Mesh Resource Queue..
	std::queue<MaterialBuffer*>		m_PushMaterialList;						// Game Engine 측에서 현재 프레임에 생성한 Material Resource Queue..
	std::queue<MaterialBuffer*>		m_ChangeMaterialList;					// Game Engine 측에서 현재 프레임에 변경한 Material Resource Queue..
	std::queue<AnimationBuffer*>	m_PushAnimationList;					// Game Engine 측에서 현재 프레임에 생성한 Animation Resource Queue..
	std::queue<AnimationBuffer*>	m_ChangeAnimationList;					// Game Engine 측에서 현재 프레임에 변경한 Animation Resource Queue..

	std::unordered_map<UINT, MeshRenderBuffer*>			m_MeshList;			// 현재 등록 되어있는 Mesh Resource..
	std::unordered_map<UINT, MaterialRenderBuffer*>		m_MaterialList;		// 현재 등록 되어있는 Material Resource..
	std::unordered_map<UINT, TerrainRenderBuffer*>		m_TerrainList;		// 현재 등록 되어있는 Terrain Resource..
	std::unordered_map<UINT, UIRenderBuffer*>			m_UIList;			// 현재 등록 되어있는 UI Resource..
	std::unordered_map<UINT, AnimationRenderBuffer*>	m_AnimationList;	// 현재 등록 되어있는 Animation Resource..

	std::unordered_map<UINT, RenderData*> m_RenderList;				// 현재 등록 되어있는 모든 Render Data..

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;			// Instance Index 관리를 위한 List..
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;	// 현재 등록 되어있는 Instance Resource..
	std::unordered_map<UINT, InstanceLayer*> m_InstanceLayerList;	// Instance Resource를 공유하는 Mesh List Layer..
};

template<typename Origin, typename Convert>
inline void RenderDataConverter::ConvertPushResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList)
{
	// Material Index..
	UINT resourceIndex = origin->BufferIndex;

	// 해당 Index Resource 체크..
	auto itor = convertList.find(resourceIndex);
	assert(itor == convertList.end());

	// 새로운 Resource Render Buffer 생성..
	Convert* newResource = new Convert();

	// Resource Render Buffer 변환..
	ConvertResource<Origin, Convert>(origin, newResource);

	// Resource Render Buffer 삽입..
	convertList.insert(std::pair<UINT, Convert*>(resourceIndex, newResource));
}

template<typename Origin, typename Convert>
inline void RenderDataConverter::ConvertChangeResource(Origin* origin, std::unordered_map<UINT, Convert*>& convertList)
{
	// Resource Index..
	UINT resourceIndex = origin->BufferIndex;

	// 해당 Index Resource 체크..
	auto itor = convertList.find(resourceIndex);
	assert(itor != convertList.end());

	// Resource Render Buffer 재설정..
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
