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
	case OBJECT_TYPE::UI:
	{
		renderData->m_UI = new UIRenderBuffer();

		m_UIList.insert(std::make_pair(originData->UI_Buffer->BufferIndex, renderData->m_UI));

		// Render Data ��ȯ �� ���..
		ConvertUIData(originData, renderData);
	}
	break;
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

		// Render Data ��ȯ �� ���..
		ConvertInstanceData(originData, renderData);
	}
	break;
	default:
		// Render Data ��ȯ �� ���..
		ConvertInstanceData(originData, renderData);
		break;
	}
	
	// Render Data List ����..
	m_RenderList.insert(std::pair<UINT, RenderData*>(renderData->m_ObjectData->ObjectIndex + 1, renderData));
}

void RenderDataConverter::ConvertInstanceData(MeshData* originData, RenderData* renderData)
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
	renderData->m_Mesh		= convertMesh;
	renderData->m_Material	= convertMaterial;
	renderData->m_Animation = convertAnimation;

	// Mesh & Material & Animation Buffer ���� Instance Layer �˻� �� ���..
	RegisterInstanceLayer(renderData, convertMesh, convertMaterial, convertAnimation);
}

void RenderDataConverter::ConvertUIData(MeshData* originData, RenderData* renderData)
{
	renderData->m_UI->m_BufferIndex = originData->UI_Buffer->BufferIndex;
	renderData->m_UI->m_BufferLayer = originData->UI_Buffer->BufferLayer;
	renderData->m_UI->m_UIProperty	= originData->UI_Buffer->UI_Property;
	renderData->m_UI->m_Albedo		= (originData->UI_Buffer->Albedo == nullptr) ? nullptr : (ID3D11ShaderResourceView*)originData->UI_Buffer->Albedo->pTextureBuf;

	// �ش� UI Layer �˻� �� ���..
	RegisterUILayer(renderData, renderData->m_UI);
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

	// Material ���� Layer ���ġ ����..
	if (convert->m_MaterialProperty->IsRelocation)
	{
		// Layer ���ġ ����..
		m_RenderManager->RelocationLayer(convert);

		// Layer ���ġ ���� ����..
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
	// �ش� Index Render Data üũ..
	std::unordered_map<UINT, RenderData*>::iterator renderData_itor = m_RenderList.find(index);

	// �ش� Render Data�� ���µ� ������� ���� ������Ѵ�..
	assert(renderData_itor != m_RenderList.end());

	// �ش� Render Data �˻�..
	RenderData* renderData = renderData_itor->second;

	// �ش� Object Type�� ���� �߰� �۾�..
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

	// �ش� Render Data ����..
	SAFE_DELETE(renderData);
	m_RenderList.erase(index);
}

void RenderDataConverter::DeleteInstance(UINT index)
{

}

void RenderDataConverter::DeleteMesh(UINT index)
{
	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	// �ش� Mesh�� ���µ� ������� ���� ������Ѵ�..
	assert(itor != m_MeshList.end());

	// �ش� Mesh �˻�..
	MeshRenderBuffer* mesh = itor->second;

	// �ش� Mesh ���� Layer ����..
	DeleteResourceLayer(mesh);

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

	// �ش� Material ���� Layer ����..
	DeleteResourceLayer(material);

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

	// �ش� Animation ���� Layer ����..
	DeleteResourceLayer(animation);

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

void RenderDataConverter::DeleteInstanceLayer(UINT index)
{
	// �ش� Instance �˻�..
	InstanceRenderBuffer* instance = GetInstance(index);

	// �ش� Instance Layer �˻�..
	InstanceLayer* layer = GetInstanceLayer(index);

	// �ش� Layer�� ���ٸ� ó������ �ʴ´�..
	if (instance == nullptr || layer == nullptr) return;

	// �ش� Instance Buffer ����..
	SAFE_DELETE(instance);
	m_InstanceList.erase(index);

	// �ش� Instance Layer ����..
	SAFE_DELETE(layer);
	m_InstanceLayerList.erase(index);

	// �ش� Instance Index ������� ����..
	m_InstanceLayerIndexList[index].second = false;
}

void RenderDataConverter::DeleteUILayer(UINT index)
{
	// �ش� Layer �˻�..
	UILayer* layer = GetUILayer(index);

	// �ش� Layer�� ���ٸ� ó������ �ʴ´�..
	if (layer == nullptr) return;

	// �ش� UI Layer ����..
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
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;
			return;
		}
	}

	// �߰��� Instance Layer Index �ο�..
	UINT layer_Index = 0;

	for (int i = 0; i < m_InstanceLayerIndexList.size(); i++)
	{
		// Index List�� ����� �ִٸ� �ش� Index �ο�..
		if (m_InstanceLayerIndexList[i].second == false)
		{
			layer_Index = m_InstanceLayerIndexList[i].first;
			m_InstanceLayerIndexList[i].second = true;
			break;
		}
	}

	// ���� Index List�� ����� ���ٸ� ���� Index �߰�..
	if (layer_Index == 0)
	{
		layer_Index = (UINT)m_InstanceLayerIndexList.size();
		m_InstanceLayerIndexList.push_back(std::pair<UINT, bool>(layer_Index, true));
	}

	// ���ο� Instance Buffer ����..
	instanceBuffer = new InstanceRenderBuffer();

	// ���� Instance Index ����..
	instanceBuffer->m_BufferIndex = layer_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;
	instanceBuffer->m_Animation = animation;

	// Instance List �߰�..
	m_InstanceList.insert(std::make_pair(layer_Index, instanceBuffer));

	// ���ο� Instance Layer ����..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = layer_Index;
	instanceLayer->m_Instance = instanceBuffer;

	// Instance Layer �߰�..
	m_InstanceLayerList.insert(std::make_pair(layer_Index, instanceLayer));

	// Instance Index ����..
	renderData->m_InstanceLayerIndex = layer_Index;
}

void RenderDataConverter::RegisterUILayer(RenderData* renderData, UIRenderBuffer* ui)
{
	// �ش� UI Layer Index..
	UINT layer_Index = ui->m_BufferLayer;

	// �ش� UI Layer�� ��ϵǾ� �ִ��� �˻�..
	UILayer* ui_layer = GetUILayer(layer_Index);

	// �ش� UI Layer�� �������� �ʴ´ٸ� �ش� Layer �߰�..
	if (ui_layer) return;

	// ���ο� UI Layer ����..
	ui_layer = new UILayer();
	ui_layer->m_LayerIndex = ui->m_BufferLayer;

	// UI Layer �߰�..
	m_UILayerList.insert(std::make_pair(layer_Index, ui_layer));
}

void RenderDataConverter::DeleteResourceLayer(MeshRenderBuffer* mesh)
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

void RenderDataConverter::DeleteResourceLayer(MaterialRenderBuffer* material)
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

void RenderDataConverter::DeleteResourceLayer(AnimationRenderBuffer* animation)
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
