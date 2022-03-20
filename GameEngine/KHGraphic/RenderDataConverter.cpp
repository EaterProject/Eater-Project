#include "DirectDefine.h"
#include "EngineData.h"
#include "RenderData.h"
#include "RenderDataConverter.h"

RenderDataConverter::RenderDataConverter()
{

}

RenderDataConverter::~RenderDataConverter()
{
	Release();
}

RenderData* RenderDataConverter::ConvertRenderData(MeshData* originData)
{
	// ���ο� Renderer ���� Render Data ����..
	RenderData* renderData = new RenderData();

	// Mesh Data ����..
	renderData->m_ObjectData = originData->Object_Data;
	renderData->m_ParticleData = originData->Particle_Data;
	renderData->m_ColliderData = originData->Collider_Data;

	// ��ȯ�� Render Data ����..
	originData->Render_Data = renderData;

	// �ش� Instance Data ����..
	MeshBuffer* originMesh = originData->Mesh_Buffer;
	MaterialBuffer* originMaterial = originData->Material_Buffer;

	// Mesh Ȥ�� Material ���� �ϳ��� ������ Buffer �������� �ʴ´�..
	if (originMesh == nullptr || originMaterial == nullptr)
	{
		return renderData;
	}

	// �ش� Mesh & Material Buffer ����..
	MeshRenderBuffer* convertMesh = GetMesh(originMesh->BufferIndex);
	MaterialRenderBuffer* convertMaterial = GetMaterial(originMaterial->BufferIndex);

	// Mesh & Material Buffer ���� Instance ����..
	InstanceRenderBuffer* convertInstance = FindInstance(renderData, convertMesh, convertMaterial);
	
	// Obejct Type�� ���� �߰� ��ȯ �۾�..
	switch (convertInstance->m_Type)
	{
	case OBJECT_TYPE::TERRAIN:
	{
		renderData->m_TerrainBuffer = new TerrainRenderBuffer();
		renderData->m_TerrainBuffer->m_Tex = &originData->Terrain_Data->Tex;

		for (MaterialBuffer* layer : originData->Terrain_Data->Material_List)
		{
			// �ش� Material ã��..
			MaterialRenderBuffer* layerMaterial = GetMaterial(layer->BufferIndex);

			// Material List �߰�..
			renderData->m_TerrainBuffer->m_MaterialList.push_back(layerMaterial);
		}

		m_LayerList.insert(std::make_pair(m_LayerList.size(), renderData->m_TerrainBuffer));
	}
	break;
	default:
		break;
	}

	return renderData;
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

void RenderDataConverter::PushChangeMesh(MeshBuffer* mesh)
{
	m_ChangeMeshList.push(mesh);
}

void RenderDataConverter::PushChangeMaterial(MaterialBuffer* material)
{
	m_ChangeMaterialList.push(material);
}

void RenderDataConverter::ConvertPushResource()
{
	// ���� ������ ���� �׾Ƶ� Push Mesh Queue ó��..
	while (!m_PushMeshList.empty())
	{
		// ���� �߰��� Mesh Buffer..
		MeshBuffer* mesh = m_PushMeshList.front();

		// �߰��� Resource ��ȯ..
		ConvertPushMesh(mesh);

		// ��ȯ�� Mesh Buffer Pop..
		m_PushMeshList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Push Material Queue ó��..
	while (!m_PushMaterialList.empty())
	{
		// ���� �߰��� Material Buffer..
		MaterialBuffer* material = m_PushMaterialList.front();

		// �߰��� Resource ��ȯ..
		ConvertPushMaterial(material);

		// ��ȯ�� Material Buffer Pop..
		m_PushMaterialList.pop();
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
		ConvertChangeMesh(mesh);

		// ��ȯ�� Mesh Buffer Pop..
		m_ChangeMeshList.pop();
	}

	// ���� ������ ���� �׾Ƶ� Change Material Queue ó��..
	while (!m_ChangeMaterialList.empty())
	{
		// ���� ������ Resource�� �ִ� Material Buffer..
		MaterialBuffer* material = m_ChangeMaterialList.front();

		// ������ Resource ��ȯ..
		ConvertChangeMaterial(material);

		// ��ȯ�� Material Buffer Pop..
		m_ChangeMaterialList.pop();
	}
}

void RenderDataConverter::ConvertPushMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// �ش� Mesh�� ���� �Ǵ� ���� ������Ѵ�..
	assert(itor == m_MeshList.end());

	// ���ο� Mesh Render Buffer ����..
	MeshRenderBuffer* newMesh = new MeshRenderBuffer();

	// Mesh Render Buffer ��ȯ..
	ConvertMesh(mesh, newMesh);

	// Mesh Render Buffer ����..
	m_MeshList.insert(std::pair<UINT, MeshRenderBuffer*>(meshIndex, newMesh));
}

void RenderDataConverter::ConvertPushMaterial(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// �ش� Index Material üũ..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// �ش� Material�� ���� �Ǵ� ���� ������Ѵ�..
	assert(itor == m_MaterialList.end());

	// ���ο� Material Render Buffer ����..
	MaterialRenderBuffer* newMaterial = new MaterialRenderBuffer();

	// Material �⺻ Data ����..
	newMaterial->m_MaterialSubData = material->Material_SubData;

	// Material Render Buffer ��ȯ..
	ConvertMaterial(material, newMaterial);

	// Material Render Buffer ����..
	m_MaterialList.insert(std::pair<UINT, MaterialRenderBuffer*>(materialIndex, newMaterial));
}

void RenderDataConverter::ConvertChangeMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// �ش� Mesh�� ���� ���� ������Ѵ�..
	assert(itor != m_MeshList.end());

	// Mesh Render Buffer �缳��..
	ConvertMesh(mesh, itor->second);
}

void RenderDataConverter::ConvertChangeMaterial(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// �ش� Index Material üũ..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// �ش� Material�� ���� ���� ������Ѵ�..
	assert(itor != m_MaterialList.end());

	// Material Render Buffer �缳��..
	ConvertMaterial(material, itor->second);
}

void RenderDataConverter::ConvertMesh(MeshBuffer* originBuf, MeshRenderBuffer* convertData)
{
	if (originBuf == nullptr) return;

	// Index Buffer Data Convert..
	convertData->m_IndexCount = originBuf->IndexBuf->Count;
	convertData->m_IndexBuf = (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	convertData->m_Stride = originBuf->VertexBuf->Stride;
	convertData->m_VertexBuf = (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderDataConverter::ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat)
{
	if (originMat == nullptr) return;

	// �ش� Material Data ��ȯ..
	if (originMat->Albedo) convertMat->m_Albedo = (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal = (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM = (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;
}

InstanceRenderBuffer* RenderDataConverter::FindInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material)
{
	ObjectData* object = renderData->m_ObjectData;

	// �ش� Mesh & Material ���� Instance Check..
	InstanceRenderBuffer* instance = CheckInstance(mesh, material);

	// ���� Instance�� �����Ѵٸ� �������� �ʴ´�..
	if (instance != nullptr)
	{
		// Instance Index ����..
		renderData->m_InstanceIndex		 = instance->m_BufferIndex;
		renderData->m_InstanceLayerIndex = instance->m_BufferIndex;

		// Render Data �߰�..

		return instance;
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
		instance_Index = m_InstanceIndexList.size();
		m_InstanceIndexList.push_back(std::pair<UINT, bool>(instance_Index, true));
	}

	// ���ο� Instance Buffer ����..
	instance = new InstanceRenderBuffer();

	// ���� Instance Index ����..
	instance->m_BufferIndex = instance_Index;
	instance->m_Type = (UINT)object->ObjType;
	instance->m_Mesh = mesh;
	instance->m_Material = material;

	// Instance List �߰�..
	m_InstanceList.insert(std::make_pair(instance_Index, instance));

	// ���ο� Instance Layer ����..
	InstanceLayer* layer = new InstanceLayer();
	layer->m_LayerIndex = instance_Index;
	layer->m_Instance = instance;

	// Instance Layer �߰�..
	m_InstanceLayerList.insert(std::make_pair(instance_Index, layer));

	// Instance Index ����..
	renderData->m_InstanceIndex = instance_Index;
	renderData->m_InstanceLayerIndex = instance_Index;

	return instance;
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

	// �ش� Instance Buffer ����..
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

	// �ش� Instance Buffer ����..
	SAFE_DELETE(material);
	m_MaterialList.erase(index);
}

MeshRenderBuffer* RenderDataConverter::GetMesh(UINT index)
{
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	if (itor == m_MeshList.end()) return nullptr;

	return itor->second;
}

MaterialRenderBuffer* RenderDataConverter::GetMaterial(UINT index)
{
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(index);

	if (itor == m_MaterialList.end()) return nullptr;

	return itor->second;
}

InstanceRenderBuffer* RenderDataConverter::GetInstance(UINT index)
{
	std::unordered_map<UINT, InstanceRenderBuffer*>::iterator itor = m_InstanceList.find(index);

	if (itor == m_InstanceList.end()) return nullptr;

	return itor->second;
}

InstanceLayer* RenderDataConverter::GetLayer(UINT index)
{
	std::unordered_map<UINT, InstanceLayer*>::iterator itor = m_InstanceLayerList.find(index);

	if (itor == m_InstanceLayerList.end()) return nullptr;

	return itor->second;
}

InstanceRenderBuffer* RenderDataConverter::CheckInstance(MeshRenderBuffer* mesh, MaterialRenderBuffer* material)
{
	InstanceRenderBuffer* instanceBuffer = nullptr;

	// Instance List���� ������ Instance Buffer ã��..
	for (auto& instance : m_InstanceList)
	{
		// ���� �˻��� Instance Buffer..
		instanceBuffer = instance.second;

		// Mesh Buffer�� Material Buffer�� ���� ��� ���� Instance ���..
		if (instanceBuffer->m_Mesh == mesh && instanceBuffer->m_Material == material)
		{
			return instanceBuffer;
		}
	}

	return nullptr;
}

void RenderDataConverter::CheckEmptyInstance(MeshRenderBuffer* mesh)
{
	std::queue<UINT> deleteIndex;

	// Instance Layer ���� ��� �˻�..
	for (auto& instance : m_InstanceLayerList)
	{
		if (instance.second->m_Instance->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// �ش� Layer ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		delete instance->second;

		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance ���� ��� �˻�..
	for (auto& instance : m_InstanceList)
	{
		if (instance.second->m_Mesh == mesh)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// �ش� Instance ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();
		
		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);
		
		delete instance->second;

		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}

void RenderDataConverter::CheckEmptyInstance(MaterialRenderBuffer* material)
{
	std::queue<UINT> deleteIndex;

	// Layer ���� ��� �˻�..
	for (auto& instance : m_InstanceLayerList)
	{
		if (instance.second->m_Instance->m_Material == material)
		{
			deleteIndex.push(instance.second->m_LayerIndex);
		}
	}

	// �ش� Layer ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		std::unordered_map<UINT, InstanceLayer*>::iterator instance = m_InstanceLayerList.find(index);

		SAFE_DELETE(instance->second);

		m_InstanceLayerList.erase(index);

		deleteIndex.pop();
	}

	// Instance ���� ��� �˻�..
	for (auto& instance : m_InstanceList)
	{
		if (instance.second->m_Material == material)
		{
			deleteIndex.push(instance.second->m_BufferIndex);
		}
	}

	// �ش� Instance ����..
	while (!deleteIndex.empty())
	{
		UINT index = deleteIndex.front();

		std::unordered_map<UINT, InstanceRenderBuffer*>::iterator instance = m_InstanceList.find(index);

		SAFE_DELETE(instance->second);

		m_InstanceList.erase(index);

		deleteIndex.pop();
	}
}
