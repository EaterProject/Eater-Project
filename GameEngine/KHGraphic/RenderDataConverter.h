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
/// 
/// - Render Resource 등록 및 동기화 순서
///  1) 현재 Frame에 추가되거나 변경된 Resource 각각의 Queue에 삽입해둔다.
///  2) 생성 -> 변경 순으로 Resource Update 하므로 생성된 Resource를 변경하더라도 순서 보장 가능.
///  3) Renderer 측에 호출되기 전에 현재 Frame의 Update는 마친 상태이므로
///     어느 부분에 추가를 하던 Update 후 처리를 하여 Rendering을 할 것이므로 순서보장 가능.
///  4) 실질적인 Resource Instance Layer 관리는 위 클래스에서 하고 
///     해당 Layer를 참조하여 Render Manager에서 Rendering만 한다.

class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertMeshData(MeshData* originData, RenderData* renderData);		// Mesh Data -> Render Data 변환..
	void ConvertRenderData(MeshData* originData, RenderData* renderData);	// Mesh Data -> Render Data 변환..
	
	void ResourceUpdate();		// 현재 프레임에 생성 & 삽입 Resource Update..

	void Release();

public:
	void PushMesh(MeshBuffer* mesh);					// 현재 프레임에 생성된 Mesh 임시 Queue에 삽입..
	void PushMaterial(MaterialBuffer* material);		// 현재 프레임에 생성된 Material 임시 Queue에 삽입..
	void PushChangeMesh(MeshBuffer* mesh);				// 현재 프레임에 변경된 Mesh 임시 Queue에 삽입..
	void PushChangeMaterial(MaterialBuffer* material);  // 현재 프레임에 변경된 Material 임시 Queue에 삽입..

public:
	void DeleteRenderData(UINT index);		// 해당 Render Data 즉시 제거..
	void DeleteInstance(UINT index);		// 해당 Instance Resource 즉시 제거..
	void DeleteMesh(UINT index);			// 해당 Mesh Resource 즉시 제거..
	void DeleteMaterial(UINT index);		// 해당 Material Resource 즉시 제거..

public:
	RenderData* GetRenderData(UINT index);			// Render Data 고유 Index로 검색..
	MeshRenderBuffer* GetMesh(UINT index);			// Mesh Resource 고유 Index로 검색..
	MaterialRenderBuffer* GetMaterial(UINT index);	// Material Resource 고유 Index로 검색..
	InstanceRenderBuffer* GetInstance(UINT index);	// Instance Resource 고유 Index로 검색..
	InstanceLayer* GetLayer(UINT index);			// Layer Resource 고유 Index로 검색..

private:
	void ConvertPushResource();			// 현재 프레임에 생성된 Resource Update..
	void ConvertChangeResource();		// 현재 프레임에 변경된 Resource Update..

	void ConvertPushMesh(MeshBuffer* mesh);					// 생성된 Mesh Resource 변환 후 등록..
	void ConvertPushMaterial(MaterialBuffer* material);		// 생성된 Material Resource 변환 후 등록..

	void ConvertChangeMesh(MeshBuffer* mesh);				// 변경된 Mesh Resource 변환..
	void ConvertChangeMaterial(MaterialBuffer* material);	// 변경된 Material Resource 변환..

	void ConvertMesh(MeshBuffer* originBuf, MeshRenderBuffer* convertData);				// 생성된 Mesh Resource 변환..
	void ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat);	// 생성된 Material Resource 변환..

	void RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material);		// Mesh & Material Resource 기준 Instance 검색 및 등록..
	
	void CheckEmptyInstance(MeshRenderBuffer* mesh);			// Mesh Resource 삭제시 관련된 Instance 삭제..
	void CheckEmptyInstance(MaterialRenderBuffer* material);	// Material Resource 삭제시 관련된 Instance 삭제..

private:
	std::queue<MeshBuffer*> m_PushMeshList;				// Game Engine 측에서 현재 프레임에 생성한 Mesh Resource Queue..
	std::queue<MeshBuffer*> m_ChangeMeshList;			// Game Engine 측에서 현재 프레임에 변경한 Mesh Resource Queue..
	std::queue<MaterialBuffer*> m_PushMaterialList;		// Game Engine 측에서 현재 프레임에 생성한 Material Resource Queue..
	std::queue<MaterialBuffer*> m_ChangeMaterialList;	// Game Engine 측에서 현재 프레임에 변경한 Material Resource Queue..

	std::unordered_map<UINT, MeshRenderBuffer*> m_MeshList;			// 현재 등록 되어있는 Mesh Resource..
	std::unordered_map<UINT, MaterialRenderBuffer*> m_MaterialList;	// 현재 등록 되어있는 Material Resource..
	std::unordered_map<UINT, TerrainRenderBuffer*> m_LayerList;		// 현재 등록 되어있는 Terrain Resource..

	std::unordered_map<UINT, RenderData*> m_RenderList;				// 현재 등록 되어있는 모든 Render Data..

	std::vector<std::pair<UINT, bool>> m_InstanceIndexList;			// Instance Index 관리를 위한 List..
	std::unordered_map<UINT, InstanceRenderBuffer*> m_InstanceList;	// 현재 등록 되어있는 Instance Resource..
	std::unordered_map<UINT, InstanceLayer*> m_InstanceLayerList;	// Instance Resource를 공유하는 Mesh List Layer..
};

