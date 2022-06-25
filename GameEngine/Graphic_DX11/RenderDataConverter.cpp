#include "DirectDefine.h"
#include "EngineData.h"
#include "RenderData.h"
#include "RenderDataConverter.h"
#include "RenderManagerBase.h"

RenderDataConverter::RenderDataConverter()
{

}

RenderDataConverter::~RenderDataConverter()
{

}

void RenderDataConverter::Initialize(IRenderManager* renderManager)
{
	m_RenderManager = renderManager;
}

void RenderDataConverter::ConvertMeshData(MeshData* originData, RenderData* renderData)
{
	// Mesh Data 설정..
	renderData->m_ObjectData		= originData->Object_Data;
	renderData->m_AnimationData		= originData->Animation_Data;
	renderData->m_ParticleData		= originData->Particle_Data;

	// ID 설정을 위한 Hash Color 생성..
	// UINT 형식으로 찍을 것이기에 0번 인덱스는 비워둔다..
	renderData->m_ObjectData->HashColor = ObjectData::HashToColor(renderData->m_ObjectData->ObjectIndex + 1);

	// 변환된 Render Data 저장..
	originData->Render_Data = (void*)renderData;

	// Obejct Type에 따른 추가 변환 작업..
	switch (originData->Object_Data->ObjType)
	{
	case OBJECT_TYPE::UI:
	{
		renderData->m_UI = new UIRenderBuffer();

		m_UIList.insert(std::make_pair(originData->UI_Buffer->BufferIndex, renderData->m_UI));

		// Render Data 변환 및 등록..
		ConvertUIData(originData, renderData);
	}
	break;
	case OBJECT_TYPE::TERRAIN:
	{
		renderData->m_Terrain = new TerrainRenderBuffer();
		renderData->m_Terrain->m_Tex = &originData->Terrain_Data->Tex;

		for (MaterialBuffer* layer : originData->Terrain_Data->Material_List)
		{
			// 해당 Material 찾기..
			MaterialRenderBuffer* layerMaterial = GetMaterial(layer->BufferIndex);

			// Material List 추가..
			renderData->m_Terrain->m_MaterialList.push_back(layerMaterial);
		}

		m_TerrainList.insert(std::make_pair((UINT)m_TerrainList.size(), renderData->m_Terrain));

		// Render Data 변환 및 등록..
		ConvertInstanceData(originData, renderData);
	}
	break;
	default:
		// Render Data 변환 및 등록..
		ConvertInstanceData(originData, renderData);
		break;
	}
	
	// Render Data List 삽입..
	m_RenderList.insert(std::pair<UINT, RenderData*>(renderData->m_ObjectData->ObjectIndex + 1, renderData));
}

void RenderDataConverter::ConvertInstanceData(MeshData* originData, RenderData* renderData)
{
	// 해당 Instance Data 추출..
	int meshIndex		= (originData->Mesh_Buffer == nullptr)		? -1 : originData->Mesh_Buffer->BufferIndex;
	int materialIndex	= (originData->Material_Buffer == nullptr)	? -1 : originData->Material_Buffer->BufferIndex;
	int animationIndex	= (originData->Animation_Buffer == nullptr) ? -1 : originData->Animation_Buffer->BufferIndex;

	// Mesh 혹은 Material 둘중 하나라도 없으면 Buffer 생성하지 않는다..
	if (meshIndex == -1 || materialIndex == -1) return;

	// 해당 Mesh & Material Buffer 설정..
	MeshRenderBuffer* convertMesh = GetMesh(meshIndex);
	MaterialRenderBuffer* convertMaterial = GetMaterial(materialIndex);
	AnimationRenderBuffer* convertAnimation = GetAnimation(animationIndex);

	// Render Data 삽입..
	renderData->m_Mesh		= convertMesh;
	renderData->m_Material	= convertMaterial;
	renderData->m_Animation = convertAnimation;

	// Mesh & Material & Animation Buffer 기준 Instance Layer 검색 및 등록..
	RegisterInstanceLayer(renderData, convertMesh, convertMaterial, convertAnimation);
}

void RenderDataConverter::ConvertUIData(MeshData* originData, RenderData* renderData)
{
	renderData->m_UI->m_BufferIndex = originData->UI_Buffer->BufferIndex;
	renderData->m_UI->m_BufferLayer = originData->UI_Buffer->BufferLayer;
	renderData->m_UI->m_UIProperty	= originData->UI_Buffer->UI_Property;
	renderData->m_UI->m_Albedo		= (originData->UI_Buffer->Albedo == nullptr) ? nullptr : (ID3D11ShaderResourceView*)originData->UI_Buffer->Albedo->pTextureBuf;

	// 해당 UI Layer 검색 및 등록..
	RegisterUILayer(renderData, renderData->m_UI);
}

void RenderDataConverter::ResourceUpdate()
{
	// 현재 프레임에 추가된 Resource 변환..
	ConvertPushResource();

	// 현재 프레임에 변경된 Resource 변환..
	ConvertChangeResource();
}

void RenderDataConverter::Release()
{

}

void RenderDataConverter::PushMesh(MeshBuffer* mesh)
{
	m_PushMeshList.push(mesh);
}

void RenderDataConverter::PushMaterial(MaterialBuffer* material)
{
	m_PushMaterialList.push(material);
}

void RenderDataConverter::PushAnimation(AnimationBuffer* animation)
{
	m_PushAnimationList.push(animation);
}

void RenderDataConverter::PushChangeMesh(MeshBuffer* mesh)
{
	m_ChangeMeshList.push(mesh);
}

void RenderDataConverter::PushChangeMaterial(MaterialBuffer* material)
{
	m_ChangeMaterialList.push(material);
}

void RenderDataConverter::PushChangeAnimation(AnimationBuffer* animation)
{
	m_ChangeAnimationList.push(animation);
}

void RenderDataConverter::ConvertPushResource()
{
	// 현재 프레임 동안 쌓아둔 Push Mesh Queue 처리..
	while (!m_PushMeshList.empty())
	{
		// 현재 추가된 Mesh Buffer..
		MeshBuffer* mesh = m_PushMeshList.front();

		// 추가된 Resource 변환..
		ConvertPushResource<MeshBuffer, MeshRenderBuffer>(mesh, m_MeshList);

		// 변환된 Mesh Buffer Pop..
		m_PushMeshList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Push Material Queue 처리..
	while (!m_PushMaterialList.empty())
	{
		// 현재 추가된 Material Buffer..
		MaterialBuffer* material = m_PushMaterialList.front();

		// 추가된 Resource 변환..
		ConvertPushResource<MaterialBuffer, MaterialRenderBuffer>(material, m_MaterialList);

		// 변환된 Material Buffer Pop..
		m_PushMaterialList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Push Animation Queue 처리..
	while (!m_PushAnimationList.empty())
	{
		// 현재 추가된 Animation Buffer..
		AnimationBuffer* animation = m_PushAnimationList.front();

		// 추가된 Resource 변환..
		ConvertPushResource<AnimationBuffer, AnimationRenderBuffer>(animation, m_AnimationList);

		// 변환된 Animation Buffer Pop..
		m_PushAnimationList.pop();
	}
}

void RenderDataConverter::ConvertChangeResource()
{
	// 현재 프레임 동안 쌓아둔 Change Mesh Queue 처리..
	while (!m_ChangeMeshList.empty())
	{
		// 현재 변동된 Resource가 있는 Mesh Buffer..
		MeshBuffer* mesh = m_ChangeMeshList.front();

		// 변동된 Resource 변환..
		ConvertChangeResource<MeshBuffer, MeshRenderBuffer>(mesh, m_MeshList);

		// 변환된 Mesh Buffer Pop..
		m_ChangeMeshList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Change Material Queue 처리..
	while (!m_ChangeMaterialList.empty())
	{
		// 현재 변동된 Resource가 있는 Material Buffer..
		MaterialBuffer* material = m_ChangeMaterialList.front();

		// 변동된 Resource 변환..
		ConvertChangeResource<MaterialBuffer, MaterialRenderBuffer>(material, m_MaterialList);

		// 변환된 Material Buffer Pop..
		m_ChangeMaterialList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Change Animation Queue 처리..
	while (!m_ChangeAnimationList.empty())
	{
		// 현재 추가된 Animation Buffer..
		AnimationBuffer* animation = m_ChangeAnimationList.front();

		// 추가된 Resource 변환..
		ConvertChangeResource<AnimationBuffer, AnimationRenderBuffer>(animation, m_AnimationList);

		// 변환된 Animation Buffer Pop..
		m_ChangeAnimationList.pop();
	}
}

template<>
void RenderDataConverter::ConvertResource(MeshBuffer* origin, MeshRenderBuffer* convert)
{
	if (origin == nullptr) return;

	// Mesh Buffer Index..
	convert->m_BufferIndex = origin->BufferIndex;

	// Mesh Buffer Name..
	convert->m_Name = origin->Name;

	// Mesh Sub Data Convert..
	convert->m_MeshProperty = origin->Mesh_Property;

	// Index Buffer Data Convert..
	convert->m_IndexCount = origin->IndexBuf->Count;
	convert->m_IndexBuf = (ID3D11Buffer*)origin->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	convert->m_Stride = origin->VertexBuf->Stride;
	convert->m_Offset = origin->VertexBuf->Offset;
	convert->m_VertexBuf = (ID3D11Buffer*)origin->VertexBuf->pVertexBuf;
}

template<>
void RenderDataConverter::ConvertResource(MaterialBuffer* origin, MaterialRenderBuffer* convert)
{
	if (origin == nullptr) return;

	// Material Index..
	convert->m_BufferIndex = origin->BufferIndex;

	// Material Buffer Name..
	convert->m_Name = origin->Name;

	// Material Sub Data Convert..
	convert->m_MaterialProperty = origin->Material_Property;

	// Material Data Convert..
	convert->m_Albedo	= (origin->Albedo == nullptr)	? nullptr : (ID3D11ShaderResourceView*)origin->Albedo->pTextureBuf;
	convert->m_Normal	= (origin->Normal == nullptr)	? nullptr : (ID3D11ShaderResourceView*)origin->Normal->pTextureBuf;
	convert->m_Emissive = (origin->Emissive == nullptr) ? nullptr : (ID3D11ShaderResourceView*)origin->Emissive->pTextureBuf;
	convert->m_ORM		= (origin->ORM == nullptr)		? nullptr : (ID3D11ShaderResourceView*)origin->ORM->pTextureBuf;

	// Material 관련 Layer 재배치 여부..
	if (convert->m_MaterialProperty->IsRelocation)
	{
		// Layer 재배치 실행..
		m_RenderManager->RelocationLayer(convert);

		// Layer 재배치 상태 변경..
		convert->m_MaterialProperty->IsRelocation = false;
	}
}

template<>
void RenderDataConverter::ConvertResource(AnimationBuffer* origin, AnimationRenderBuffer* convert)
{
	if (origin == nullptr) return;

	// Animation Index..
	convert->m_BufferIndex = origin->BufferIndex;

	// Animation Sub Data Convert..
	convert->m_FrameOffset = origin->FrameOffset;
	convert->m_AnimationOffset = origin->AnimationOffset;

	// Animation Data Convert..
	convert->m_AnimationBuf = (ID3D11ShaderResourceView*)origin->pAnimationBuf;
}

void RenderDataConverter::DeleteRenderData(UINT index)
{
	// 해당 Index Render Data 체크..
	std::unordered_map<UINT, RenderData*>::iterator renderData_itor = m_RenderList.find(index);

	// 해당 Render Data가 없는데 지우려는 경우는 없어야한다..
	assert(renderData_itor != m_RenderList.end());

	// 해당 Render Data 검색..
	RenderData* renderData = renderData_itor->second;

	// 해당 Object Type에 따른 추가 작업..
	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::UI:
	{

	}
		break;
	case OBJECT_TYPE::TERRAIN:
	{

	}
		break;
	default:
		break;
	}

	// 해당 Render Data 삭제..
	SAFE_DELETE(renderData);
	m_RenderList.erase(index);
}

void RenderDataConverter::DeleteInstance(UINT index)
{

}

void RenderDataConverter::DeleteMesh(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	// 해당 Mesh가 없는데 지우려는 경우는 없어야한다..
	assert(itor != m_MeshList.end());

	// 해당 Mesh 검색..
	MeshRenderBuffer* mesh = itor->second;

	// 해당 Mesh 관련 Layer 삭제..
	DeleteResourceLayer(mesh);

	// 해당 Resource 제거..
	RELEASE_COM(mesh->m_IndexBuf);
	RELEASE_COM(mesh->m_VertexBuf);

	// 해당 Mesh Buffer 삭제..
	SAFE_DELETE(mesh);
	m_MeshList.erase(index);

}

void RenderDataConverter::DeleteMaterial(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(index);

	// 해당 Material이 없는데 지우려는 경우는 없어야한다..
	assert(itor != m_MaterialList.end());

	// 해당 Material 검색..
	MaterialRenderBuffer* material = itor->second;

	// 해당 Material 관련 Layer 삭제..
	DeleteResourceLayer(material);

	// 해당 Material Buffer 삭제..
	SAFE_DELETE(material);
	m_MaterialList.erase(index);
}

void RenderDataConverter::DeleteAnimation(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, AnimationRenderBuffer*>::iterator itor = m_AnimationList.find(index);

	// 해당 Animation이 없는데 지우려는 경우는 없어야한다..
	assert(itor != m_AnimationList.end());

	// 해당 Animation 검색..
	AnimationRenderBuffer* animation = itor->second;

	// 해당 Animation 관련 Layer 삭제..
	DeleteResourceLayer(animation);

	// 해당 Resource 제거..
	RELEASE_COM(animation->m_AnimationBuf);

	// 해당 Animation Buffer 삭제..
	SAFE_DELETE(animation);
	m_AnimationList.erase(index);
}

void RenderDataConverter::DeleteUI(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, UIRenderBuffer*>::iterator itor = m_UIList.find(index);

	// 해당 UI가 없는데 지우려는 경우는 없어야한다..
	assert(itor != m_UIList.end());

	// 해당 UI 검색..
	UIRenderBuffer* ui = itor->second;

	// 해당 UI Buffer 삭제..
	SAFE_DELETE(ui);
	m_UIList.erase(index);
}

void RenderDataConverter::DeleteInstanceLayer(UINT index)
{
	// 해당 Instance 검색..
	InstanceRenderBuffer* instance = GetInstance(index);

	// 해당 Instance Layer 검색..
	InstanceLayer* layer = GetInstanceLayer(index);

	// 해당 Layer가 없다면 처리하지 않는다..
	if (instance == nullptr || layer == nullptr) return;

	// 해당 Instance Buffer 삭제..
	SAFE_DELETE(instance);
	m_InstanceList.erase(index);

	// 해당 Instance Layer 삭제..
	SAFE_DELETE(layer);
	m_InstanceLayerList.erase(index);

	// 해당 Instance Index 빈곳으로 설정..
	m_InstanceLayerIndexList[index].second = false;
}

void RenderDataConverter::DeleteUILayer(UINT index)
{
	// 해당 Layer 검색..
	UILayer* layer = GetUILayer(index);

	// 해당 Layer가 없다면 처리하지 않는다..
	if (layer == nullptr) return;

	// 해당 UI Layer 삭제..
	SAFE_DELETE(layer);
	m_UILayerList.erase(index);
}

size_t RenderDataConverter::FindMaxInstanceCount()
{
	size_t maxCount = 1;
	size_t layerCount = 0;

	for (auto& layer : m_InstanceLayerList)
	{
		layerCount = layer.second->m_InstanceList.size();

		// 제일 큰 Layer Count 저장..
		if (layerCount > maxCount)
		{
			maxCount = layerCount;
		}
	}

	return maxCount;
}

RenderData* RenderDataConverter::GetRenderData(int index)
{
	if (index < 0) return nullptr;

	std::unordered_map<UINT, RenderData*>::iterator itor = m_RenderList.find((UINT)index);

	if (itor == m_RenderList.end()) return nullptr;

	return itor->second;
}

MeshRenderBuffer* RenderDataConverter::GetMesh(int index)
{
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find((UINT)index);

	if (itor == m_MeshList.end()) return nullptr;

	return itor->second;
}

MaterialRenderBuffer* RenderDataConverter::GetMaterial(int index)
{
	if (index < 0) return nullptr;
	
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find((UINT)index);

	if (itor == m_MaterialList.end()) return nullptr;

	return itor->second;
}

AnimationRenderBuffer* RenderDataConverter::GetAnimation(int index)
{
	if (index < 0) return nullptr;
	
	std::unordered_map<UINT, AnimationRenderBuffer*>::iterator itor = m_AnimationList.find((UINT)index);

	if (itor == m_AnimationList.end()) return nullptr;

	return itor->second;
}

InstanceRenderBuffer* RenderDataConverter::GetInstance(int index)
{
	if (index < 0) return nullptr;
	
	std::unordered_map<UINT, InstanceRenderBuffer*>::iterator itor = m_InstanceList.find((UINT)index);

	if (itor == m_InstanceList.end()) return nullptr;

	return itor->second;
}

InstanceLayer* RenderDataConverter::GetInstanceLayer(int index)
{
	if (index < 0) return nullptr;
	
	std::unordered_map<UINT, InstanceLayer*>::iterator itor = m_InstanceLayerList.find((UINT)index);

	if (itor == m_InstanceLayerList.end()) return nullptr;

	return itor->second;
}

UILayer* RenderDataConverter::GetUILayer(int index)
{
	if (index < 0) return nullptr;

	std::unordered_map<UINT, UILayer*>::iterator itor = m_UILayerList.find((UINT)index);

	if (itor == m_UILayerList.end()) return nullptr;

	return itor->second;
}

void RenderDataConverter::RegisterInstanceLayer(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material, AnimationRenderBuffer* animation)
{
	InstanceLayer* instanceLayer = nullptr;
	InstanceRenderBuffer* instanceBuffer = nullptr;

	// Instance List에서 동일한 Instance Buffer 찾기..
	for (auto& instance : m_InstanceList)
	{
		// 현재 검색할 Instance Buffer..
		instanceBuffer = instance.second;

		// Mesh Buffer와 Material Buffer가 같을 경우 같은 Instance 취급..
		if (instanceBuffer->m_Mesh == mesh && 
			instanceBuffer->m_Material == material && 
			instanceBuffer->m_Animation == animation)
		{
			// 해당 Instance Index 삽입..
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;
			return;
		}
	}

	// 추가된 Instance Layer Index 부여..
	UINT layer_Index = 0;

	for (int i = 0; i < m_InstanceLayerIndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (m_InstanceLayerIndexList[i].second == false)
		{
			layer_Index = m_InstanceLayerIndexList[i].first;
			m_InstanceLayerIndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (layer_Index == 0)
	{
		layer_Index = (UINT)m_InstanceLayerIndexList.size();
		m_InstanceLayerIndexList.push_back(std::pair<UINT, bool>(layer_Index, true));
	}

	// 새로운 Instance Buffer 생성..
	instanceBuffer = new InstanceRenderBuffer();

	// 현재 Instance Index 설정..
	instanceBuffer->m_BufferIndex = layer_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;
	instanceBuffer->m_Animation = animation;

	// Instance List 추가..
	m_InstanceList.insert(std::make_pair(layer_Index, instanceBuffer));

	// 새로운 Instance Layer 생성..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = layer_Index;
	instanceLayer->m_Instance = instanceBuffer;

	// Instance Layer 추가..
	m_InstanceLayerList.insert(std::make_pair(layer_Index, instanceLayer));

	// Instance Index 삽입..
	renderData->m_InstanceLayerIndex = layer_Index;
}

void RenderDataConverter::RegisterUILayer(RenderData* renderData, UIRenderBuffer* ui)
{
	// 해당 UI Layer Index..
	UINT layer_Index = ui->m_BufferLayer;

	// 해당 UI Layer가 등록되어 있는지 검색..
	UILayer* ui_layer = GetUILayer(layer_Index);

	// 해당 UI Layer가 존재하지 않는다면 해당 Layer 추가..
	if (ui_layer) return;

	// 새로운 UI Layer 생성..
	ui_layer = new UILayer();
	ui_layer->m_LayerIndex = ui->m_BufferLayer;

	// UI Layer 추가..
	m_UILayerList.insert(std::make_pair(layer_Index, ui_layer));
}

void RenderDataConverter::DeleteResourceLayer(MeshRenderBuffer* mesh)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer 제거 목록 검색..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Instance->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// 해당 Instance Layer 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// 제거될 Instance Layer 검색..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// 해당 InstanceLayer 제거..
		SAFE_DELETE(instance->second);

		// Instance Layer List에서도 제거..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance 제거 목록 검색..
	for (auto& instance : m_InstanceList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// 해당 Instance 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();
		
		// 제거될 Instance Buffer 검색..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);
		
		// 해당 Instance Buffer 제거..
		SAFE_DELETE(instance->second);

		// Instance List에서도 제거..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}

void RenderDataConverter::DeleteResourceLayer(MaterialRenderBuffer* material)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer 제거 목록 검색..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Instance->m_Material == material)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// 해당 Instance Layer 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// 제거될 Instance Layer 검색..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// 해당 InstanceLayer 제거..
		SAFE_DELETE(instance->second);

		// Instance Layer List에서도 제거..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance 제거 목록 검색..
	for (auto& instance : m_InstanceList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Material == material)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// 해당 Instance 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// 제거될 Instance Buffer 검색..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);

		// 해당 Instance Buffer 제거..
		SAFE_DELETE(instance->second);

		// Instance List에서도 제거..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}

void RenderDataConverter::DeleteResourceLayer(AnimationRenderBuffer* animation)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer 제거 목록 검색..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Instance->m_Animation == animation)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// 해당 Instance Layer 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// 제거될 Instance Layer 검색..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// 해당 InstanceLayer 제거..
		SAFE_DELETE(instance->second);

		// Instance Layer List에서도 제거..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance 제거 목록 검색..
	for (auto& instance : m_InstanceList)
	{
		// Instance에 해당 Resource가 있다면 제거 목록 추가..
		if (instance.second->m_Animation == animation)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// 해당 Instance 제거..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// 제거될 Instance Buffer 검색..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);

		// 해당 Instance Buffer 제거..
		SAFE_DELETE(instance->second);

		// Instance List에서도 제거..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}
