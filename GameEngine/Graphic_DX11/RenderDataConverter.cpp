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
	// Mesh Data ����..
	renderData->m_ObjectData		= originData->Object_Data;
	renderData->m_AnimationData		= originData->Animation_Data;
	renderData->m_ParticleData		= originData->Particle_Data;

	// ID ������ ���� Hash Color ����..
	// UINT �������� ���� ���̱⿡ 0�� �ε����� ����д�..
	renderData->m_ObjectData->HashColor = ObjectData::HashToColor(renderData->m_ObjectData->ObjectIndex + 1);

	// ��ȯ�� Render Data ����..
	originData->Render_Data = (void*)renderData;

	// Obejct Type�� ���� �߰� ��ȯ �۾�..
	switch (originData->Object_Data->ObjType)
	{
	case OBJECT_TYPE::TERRAIN:
	{
		renderData->m_Terrain = new TerrainRenderBuffer();
		renderData->m_Terrain->m_Tex = &originData->Terrain_Data->Tex;

		for (MaterialBuffer* layer : originData->Terrain_Data->Material_List)
		{
			// �ش� Material ã��..
			MaterialRenderBuffer* layerMaterial = GetMaterial(layer->BufferIndex);

			// Material List �߰�..
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
	
	// Render Data ��ȯ �� ���..
	ConvertRenderData(originData, renderData);

	// Render Data List ����..
	m_RenderList.insert(std::pair<UINT, RenderData*>(renderData->m_ObjectData->ObjectIndex + 1, renderData));
}

void RenderDataConverter::ConvertRenderData(MeshData* originData, RenderData* renderData)
{
	// �ش� Instance Data ����..
	int meshIndex		= (originData->Mesh_Buffer == nullptr)		? -1 : originData->Mesh_Buffer->BufferIndex;
	int materialIndex	= (originData->Material_Buffer == nullptr)	? -1 : originData->Material_Buffer->BufferIndex;
	int animationIndex	= (originData->Animation_Buffer == nullptr) ? -1 : originData->Animation_Buffer->BufferIndex;

	// Mesh Ȥ�� Material ���� �ϳ��� ������ Buffer �������� �ʴ´�..
	if (meshIndex == -1 || materialIndex == -1) return;

	// �ش� Mesh & Material Buffer ����..
	MeshRenderBuffer* convertMesh = GetMesh(meshIndex);
	MaterialRenderBuffer* convertMaterial = GetMaterial(materialIndex);
	AnimationRenderBuffer* convertAnimation = GetAnimation(animationIndex);

	// Render Data ����..
	renderData->m_Mesh = convertMesh;
	renderData->m_Material = convertMaterial;
	renderData->m_Animation = convertAnimation;

	// Mesh & Material & Animation Buffer ���� Instance �˻� �� Render Data ����..
	RegisterInstance(renderData, convertMesh, convertMaterial, convertAnimation);
}

void RenderDataConverter::ResourceUpdate()
{
	// ���� �����ӿ� �߰��� Resource ��ȯ..
	ConvertPushResource();

	// ���� �����ӿ� ����� Resource ��ȯ..
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
	// ���� ������ ���� �׾Ƶ� Push Mesh Queue ó��..
	while (!m_PushMeshList.empty())
	{
		// ���� �߰��� Mesh Buffer..
		MeshBuffer* mesh = m_PushMeshList.front();

		// �߰��� Resource ��ȯ..
		ConvertPushResource<MeshBuffer, MeshRenderBuffer>(mesh, m_MeshList);

		// ��ȯ�� Mesh Buffer Pop..
		m_PushMeshList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Push Material Queue ó��..
	while (!m_PushMaterialList.empty())
	{
		// ���� �߰��� Material Buffer..
		MaterialBuffer* material = m_PushMaterialList.front();

		// �߰��� Resource ��ȯ..
		ConvertPushResource<MaterialBuffer, MaterialRenderBuffer>(material, m_MaterialList);

		// ��ȯ�� Material Buffer Pop..
		m_PushMaterialList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Push Animation Queue ó��..
	while (!m_PushAnimationList.empty())
	{
		// ���� �߰��� Animation Buffer..
		AnimationBuffer* animation = m_PushAnimationList.front();

		// �߰��� Resource ��ȯ..
		ConvertPushResource<AnimationBuffer, AnimationRenderBuffer>(animation, m_AnimationList);

		// ��ȯ�� Animation Buffer Pop..
		m_PushAnimationList.pop();
	}
}

void RenderDataConverter::ConvertChangeResource()
{
	// ���� ������ ���� �׾Ƶ� Change Mesh Queue ó��..
	while (!m_ChangeMeshList.empty())
	{
		// ���� ������ Resource�� �ִ� Mesh Buffer..
		MeshBuffer* mesh = m_ChangeMeshList.front();

		// ������ Resource ��ȯ..
		ConvertChangeResource<MeshBuffer, MeshRenderBuffer>(mesh, m_MeshList);

		// ��ȯ�� Mesh Buffer Pop..
		m_ChangeMeshList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Change Material Queue ó��..
	while (!m_ChangeMaterialList.empty())
	{
		// ���� ������ Resource�� �ִ� Material Buffer..
		MaterialBuffer* material = m_ChangeMaterialList.front();

		// ������ Resource ��ȯ..
		ConvertChangeResource<MaterialBuffer, MaterialRenderBuffer>(material, m_MaterialList);

		// ��ȯ�� Material Buffer Pop..
		m_ChangeMaterialList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Change Animation Queue ó��..
	while (!m_ChangeAnimationList.empty())
	{
		// ���� �߰��� Animation Buffer..
		AnimationBuffer* animation = m_ChangeAnimationList.front();

		// �߰��� Resource ��ȯ..
		ConvertChangeResource<AnimationBuffer, AnimationRenderBuffer>(animation, m_AnimationList);

		// ��ȯ�� Animation Buffer Pop..
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
	// �ش� Index Render Data üũ..
	std::unordered_map<UINT, RenderData*>::iterator renderData_itor = m_RenderList.find(index);

	// �ش� Render Data�� ���µ� ������� ���� ������Ѵ�..
	assert(renderData_itor != m_RenderList.end());

	// �ش� Render Data �˻�..
	RenderData* renderData = renderData_itor->second;

	// �ش� Render Data ����..
	SAFE_DELETE(renderData);
	m_RenderList.erase(index);
}

void RenderDataConverter::DeleteInstance(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance_itor = m_InstanceList.find(index);
	std::unordered_map<UINT, InstanceLayer*>::iterator layer_itor = m_InstanceLayerList.find(index);

	// �ش� Instance�� ���µ� ������� ���� ������Ѵ�..
	assert(instance_itor != m_InstanceList.end());
	assert(layer_itor != m_InstanceLayerList.end());

	// �ش� Instance �˻�..
	InstanceRenderBuffer* instance = instance_itor->second;

	// �ش� Instance Buffer ����..
	SAFE_DELETE(instance);
	m_InstanceList.erase(index);

	// �ش� Instance Layer �˻�..
	InstanceLayer* layer = layer_itor->second;

	// �ش� Instance Layer ����..
	SAFE_DELETE(layer);
	m_InstanceLayerList.erase(index);

	// �ش� Instance Index ������� ����..
	m_InstanceIndexList[index].second = false;
}

void RenderDataConverter::DeleteMesh(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	// �ش� Mesh�� ���µ� ������� ���� ������Ѵ�..
	assert(itor != m_MeshList.end());

	// �ش� Mesh �˻�..
	MeshRenderBuffer* mesh = itor->second;

	// �ش� Mesh ���� Instance ����..
	CheckEmptyInstance(mesh);

	// �ش� Resource ����..
	RELEASE_COM(mesh->m_IndexBuf);
	RELEASE_COM(mesh->m_VertexBuf);

	// �ش� Mesh Buffer ����..
	SAFE_DELETE(mesh);
	m_MeshList.erase(index);

}

void RenderDataConverter::DeleteMaterial(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(index);

	// �ش� Material�� ���µ� ������� ���� ������Ѵ�..
	assert(itor != m_MaterialList.end());

	// �ش� Material �˻�..
	MaterialRenderBuffer* material = itor->second;

	// �ش� Material ���� Instance ����..
	CheckEmptyInstance(material);

	// �ش� Material Buffer ����..
	SAFE_DELETE(material);
	m_MaterialList.erase(index);
}

void RenderDataConverter::DeleteAnimation(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, AnimationRenderBuffer*>::iterator itor = m_AnimationList.find(index);

	// �ش� Animation�� ���µ� ������� ���� ������Ѵ�..
	assert(itor != m_AnimationList.end());

	// �ش� Animation �˻�..
	AnimationRenderBuffer* animation = itor->second;

	// �ش� Animation ���� Instance ����..
	CheckEmptyInstance(animation);

	// �ش� Resource ����..
	RELEASE_COM(animation->m_AnimationBuf);

	// �ش� Animation Buffer ����..
	SAFE_DELETE(animation);
	m_AnimationList.erase(index);
}

void RenderDataConverter::DeleteUI(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, UIRenderBuffer*>::iterator itor = m_UIList.find(index);

	// �ش� UI�� ���µ� ������� ���� ������Ѵ�..
	assert(itor != m_UIList.end());

	// �ش� UI �˻�..
	UIRenderBuffer* ui = itor->second;

	// �ش� UI Buffer ����..
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

		// ���� ū Layer Count ����..
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

	// Instance List���� ������ Instance Buffer ã��..
	for (auto& instance : m_InstanceList)
	{
		// ���� �˻��� Instance Buffer..
		instanceBuffer = instance.second;

		// Mesh Buffer�� Material Buffer�� ���� ��� ���� Instance ���..
		if (instanceBuffer->m_Mesh == mesh && 
			instanceBuffer->m_Material == material && 
			instanceBuffer->m_Animation == animation)
		{
			// �ش� Instance Index ����..
			renderData->m_InstanceIndex = instanceBuffer->m_BufferIndex;
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;
			return;
		}
	}

	// �߰��� Material Index �ο�..
	UINT instance_Index = 0;

	for (int i = 0; i < m_InstanceIndexList.size(); i++)
	{
		// Index List�� ����� �ִٸ� �ش� Index �ο�..
		if (m_InstanceIndexList[i].second == false)
		{
			instance_Index = m_InstanceIndexList[i].first;
			m_InstanceIndexList[i].second = true;
			break;
		}
	}

	// ���� Index List�� ����� ���ٸ� ���� Index �߰�..
	if (instance_Index == 0)
	{
		instance_Index = (UINT)m_InstanceIndexList.size();
		m_InstanceIndexList.push_back(std::pair<UINT, bool>(instance_Index, true));
	}

	// ���ο� Instance Buffer ����..
	instanceBuffer = new InstanceRenderBuffer();

	// ���� Instance Index ����..
	instanceBuffer->m_BufferIndex = instance_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;
	instanceBuffer->m_Animation = animation;

	// Instance List �߰�..
	m_InstanceList.insert(std::make_pair(instance_Index, instanceBuffer));

	// ���ο� Instance Layer ����..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = instance_Index;
	instanceLayer->m_Instance = instanceBuffer;

	// Instance Layer �߰�..
	m_InstanceLayerList.insert(std::make_pair(instance_Index, instanceLayer));

	// Instance Index ����..
	renderData->m_InstanceIndex = instance_Index;
	renderData->m_InstanceLayerIndex = instance_Index;
}

void RenderDataConverter::CheckEmptyInstance(MeshRenderBuffer* mesh)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer ���� ��� �˻�..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Instance->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// �ش� Instance Layer ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// ���ŵ� Instance Layer �˻�..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// �ش� InstanceLayer ����..
		SAFE_DELETE(instance->second);

		// Instance Layer List������ ����..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance ���� ��� �˻�..
	for (auto& instance : m_InstanceList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// �ش� Instance ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();
		
		// ���ŵ� Instance Buffer �˻�..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);
		
		// �ش� Instance Buffer ����..
		SAFE_DELETE(instance->second);

		// Instance List������ ����..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}

void RenderDataConverter::CheckEmptyInstance(MaterialRenderBuffer* material)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer ���� ��� �˻�..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Instance->m_Material == material)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// �ش� Instance Layer ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// ���ŵ� Instance Layer �˻�..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// �ش� InstanceLayer ����..
		SAFE_DELETE(instance->second);

		// Instance Layer List������ ����..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance ���� ��� �˻�..
	for (auto& instance : m_InstanceList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Material == material)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// �ش� Instance ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// ���ŵ� Instance Buffer �˻�..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);

		// �ش� Instance Buffer ����..
		SAFE_DELETE(instance->second);

		// Instance List������ ����..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}

void RenderDataConverter::CheckEmptyInstance(AnimationRenderBuffer* animation)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer ���� ��� �˻�..
	for (auto& instance : m_InstanceLayerList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Instance->m_Animation == animation)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// �ش� Instance Layer ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// ���ŵ� Instance Layer �˻�..
		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		// �ش� InstanceLayer ����..
		SAFE_DELETE(instance->second);

		// Instance Layer List������ ����..
		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance ���� ��� �˻�..
	for (auto& instance : m_InstanceList)
	{
		// Instance�� �ش� Resource�� �ִٸ� ���� ��� �߰�..
		if (instance.second->m_Animation == animation)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// �ش� Instance ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		// ���ŵ� Instance Buffer �˻�..
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);

		// �ش� Instance Buffer ����..
		SAFE_DELETE(instance->second);

		// Instance List������ ����..
		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}
