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
	renderData->m_ObjectData = originData->Object_Data;
	renderData->m_ParticleData = originData->Particle_Data;
	renderData->m_ColliderData = &originData->Collider_Data;

	// ��ȯ�� Render Data ����..
	originData->Render_Data = (void*)renderData;

	// Obejct Type�� ���� �߰� ��ȯ �۾�..
	switch (originData->Object_Data->ObjType)
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

		m_LayerList.insert(std::make_pair((UINT)m_LayerList.size(), renderData->m_TerrainBuffer));
	}
	break;
	default:
		break;
	}
	
	// Render Data ��ȯ �� ���..
	ConvertRenderData(originData, renderData);
}

void RenderDataConverter::ConvertRenderData(MeshData* originData, RenderData* renderData)
{
	// �ش� Instance Data ����..
	MeshBuffer* originMesh = originData->Mesh_Buffer;
	MaterialBuffer* originMaterial = originData->Material_Buffer;

	// Mesh Ȥ�� Material ���� �ϳ��� ������ Buffer �������� �ʴ´�..
	if (originMesh == nullptr || originMaterial == nullptr) return;

	// �ش� Mesh & Material Buffer ����..
	MeshRenderBuffer* convertMesh = GetMesh(originMesh->BufferIndex);
	MaterialRenderBuffer* convertMaterial = GetMaterial(originMaterial->BufferIndex);

	// Mesh & Material Buffer ���� Instance �˻� �� Render Data ����..
	RegisterInstance(renderData, convertMesh, convertMaterial);
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
	newMesh->m_BufferIndex = meshIndex;

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
	newMaterial->m_BufferIndex = materialIndex;

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

	// Mesh Buffer Name..
	convertData->m_Name = originBuf->Name;

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

	// Material Buffer Name..
	convertMat->m_Name = originMat->Name;

	// �ش� Material Data ��ȯ..
	if (originMat->Albedo)
	{
		convertMat->m_Albedo = (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	}
	else
	{
		convertMat->m_Albedo = nullptr;
	}

	if (originMat->Normal)
	{
		convertMat->m_Normal = (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	}
	else
	{
		convertMat->m_Normal = nullptr;
	}

	if (originMat->Emissive)
	{
		convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	}
	else
	{
		convertMat->m_Emissive = nullptr;
	}

	if (originMat->ORM)
	{
		convertMat->m_ORM = (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;
	}
	else
	{
		convertMat->m_ORM = nullptr;
	}
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

void RenderDataConverter::RegisterInstance(RenderData* renderData, MeshRenderBuffer* mesh, MaterialRenderBuffer* material)
{
	InstanceLayer* instanceLayer = nullptr;
	InstanceRenderBuffer* instanceBuffer = nullptr;

	// Instance List���� ������ Instance Buffer ã��..
	for (auto& instance : m_InstanceList)
	{
		// ���� �˻��� Instance Buffer..
		instanceBuffer = instance.second;

		// Mesh Buffer�� Material Buffer�� ���� ��� ���� Instance ���..
		if (instanceBuffer->m_Mesh == mesh && instanceBuffer->m_Material == material)
		{
			// �ش� Instance Index ����..
			renderData->m_InstanceIndex = instanceBuffer->m_BufferIndex;
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;

			// �ش� Instance Layer�� Render Data ����..
			instanceLayer = GetLayer(instanceBuffer->m_BufferIndex);
			instanceLayer->PushRenderData(renderData);

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
		instance_Index = m_InstanceIndexList.size();
		m_InstanceIndexList.push_back(std::pair<UINT, bool>(instance_Index, true));
	}

	// ���ο� Instance Buffer ����..
	instanceBuffer = new InstanceRenderBuffer();

	// ���� Instance Index ����..
	instanceBuffer->m_BufferIndex = instance_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;

	// Instance List �߰�..
	m_InstanceList.insert(std::make_pair(instance_Index, instanceBuffer));

	// ���ο� Instance Layer ����..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = instance_Index;
	instanceLayer->m_Instance = instanceBuffer;
	instanceLayer->PushRenderData(renderData);

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
