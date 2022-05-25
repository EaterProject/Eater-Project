#include "DirectDefine.h"
#include "EngineData.h"
#include "RenderData.h"
#include "RenderDataConverter.h"

RenderDataConverter::RenderDataConverter()
{

}

RenderDataConverter::~RenderDataConverter()
{

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
	}
	break;
	case OBJECT_TYPE::UI:
	{
		renderData->m_UI = new UIRenderBuffer();
		renderData->m_UI->m_BufferIndex = originData->UI_Buffer->BufferIndex;
		renderData->m_UI->m_UIProperty = originData->UI_Buffer->UI_Property;
		renderData->m_UI->m_Albedo = (originData->UI_Buffer->Albedo == nullptr) ? nullptr : (ID3D11ShaderResourceView*)originData->UI_Buffer->Albedo->pTextureBuf;

		m_UIList.insert(std::make_pair(originData->UI_Buffer->BufferIndex, renderData->m_UI));
	}
		break;
	default:
		break;
	}
	
	// Render Data 변환 및 등록..
	ConvertRenderData(originData, renderData);

	// Render Data List 삽입..
	m_RenderList.insert(std::pair<UINT, RenderData*>(renderData->m_ObjectData->ObjectIndex + 1, renderData));
}

void RenderDataConverter::ConvertRenderData(MeshData* originData, RenderData* renderData)
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
	renderData->m_Mesh = convertMesh;
	renderData->m_Material = convertMaterial;
	renderData->m_Animation = convertAnimation;

	// Mesh & Material & Animation Buffer 기준 Instance 검색 및 Render Data 삽입..
	RegisterInstance(renderData, convertMesh, convertMaterial, convertAnimation);
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

	// 해당 Render Data 삭제..
	SAFE_DELETE(renderData);
	m_RenderList.erase(index);
}

void RenderDataConverter::DeleteInstance(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance_itor = m_InstanceList.find(index);
	std::unordered_map<UINT, InstanceLayer*>::iterator layer_itor = m_InstanceLayerList.find(index);

	// 해당 Instance가 없는데 지우려는 경우는 없어야한다..
	assert(instance_itor != m_InstanceList.end());
	assert(layer_itor != m_InstanceLayerList.end());

	// 해당 Instance 검색..
	InstanceRenderBuffer* instance = instance_itor->second;

	// 해당 Instance Buffer 삭제..
	SAFE_DELETE(instance);
	m_InstanceList.erase(index);

	// 해당 Instance Layer 검색..
	InstanceLayer* layer = layer_itor->second;

	// 해당 Instance Layer 삭제..
	SAFE_DELETE(layer);
	m_InstanceLayerList.erase(index);

	// 해당 Instance Index 빈곳으로 설정..
	m_InstanceIndexList[index].second = false;
}

void RenderDataConverter::DeleteMesh(UINT index)
{
	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	// 해당 Mesh가 없는데 지우려는 경우는 없어야한다..
	assert(itor != m_MeshList.end());

	// 해당 Mesh 검색..
	MeshRenderBuffer* mesh = itor->second;

	// 해당 Mesh 관련 Instance 삭제..
	CheckEmptyInstance(mesh);

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

	// 해당 Material 관련 Instance 삭제..
	CheckEmptyInstance(material);

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

	// 해당 Animation 관련 Instance 삭제..
	CheckEmptyInstance(animation);

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

size_t RenderDataConverter::FindMaxInstanceCount()
{
	size_t maxCount = 1;
	size_t layerCount = 0;

	for (auto& layer : m_InstanceLayerList)
	{
		layerCount = layer.second->m_MeshList.size();

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

InstanceLayer* RenderDataConverter::GetLayer(int index)
{
	if (index < 0) return nullptr;
	
	std::unordered_map<UINT, InstanceLayer*>::iterator itor = m_InstanceLayerList.find((UINT)index);

	if (itor == m_InstanceLayerList.end()) return nullptr;

	return itor->second;
}

void RenderDataConverter::RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material, AnimationRenderBuffer* animation)
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
			renderData->m_InstanceIndex = instanceBuffer->m_BufferIndex;
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;
			return;
		}
	}

	// 추가된 Material Index 부여..
	UINT instance_Index = 0;

	for (int i = 0; i < m_InstanceIndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (m_InstanceIndexList[i].second == false)
		{
			instance_Index = m_InstanceIndexList[i].first;
			m_InstanceIndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (instance_Index == 0)
	{
		instance_Index = (UINT)m_InstanceIndexList.size();
		m_InstanceIndexList.push_back(std::pair<UINT, bool>(instance_Index, true));
	}

	// 새로운 Instance Buffer 생성..
	instanceBuffer = new InstanceRenderBuffer();

	// 현재 Instance Index 설정..
	instanceBuffer->m_BufferIndex = instance_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;
	instanceBuffer->m_Animation = animation;

	// Instance List 추가..
	m_InstanceList.insert(std::make_pair(instance_Index, instanceBuffer));

	// 새로운 Instance Layer 생성..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = instance_Index;
	instanceLayer->m_Instance = instanceBuffer;

	// Instance Layer 추가..
	m_InstanceLayerList.insert(std::make_pair(instance_Index, instanceLayer));

	// Instance Index 삽입..
	renderData->m_InstanceIndex = instance_Index;
	renderData->m_InstanceLayerIndex = instance_Index;
}

void RenderDataConverter::CheckEmptyInstance(MeshRenderBuffer* mesh)
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

void RenderDataConverter::CheckEmptyInstance(MaterialRenderBuffer* material)
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

void RenderDataConverter::CheckEmptyInstance(AnimationRenderBuffer* animation)
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
