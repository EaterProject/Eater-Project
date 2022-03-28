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
	renderData->m_ObjectData = originData->Object_Data;
	renderData->m_ParticleData = originData->Particle_Data;
	renderData->m_ColliderData = &originData->Collider_Data;

	// 변환된 Render Data 저장..
	originData->Render_Data = (void*)renderData;

	// Obejct Type에 따른 추가 변환 작업..
	switch (originData->Object_Data->ObjType)
	{
	case OBJECT_TYPE::TERRAIN:
	{
		renderData->m_TerrainBuffer = new TerrainRenderBuffer();
		renderData->m_TerrainBuffer->m_Tex = &originData->Terrain_Data->Tex;

		for (MaterialBuffer* layer : originData->Terrain_Data->Material_List)
		{
			// 해당 Material 찾기..
			MaterialRenderBuffer* layerMaterial = GetMaterial(layer->BufferIndex);

			// Material List 추가..
			renderData->m_TerrainBuffer->m_MaterialList.push_back(layerMaterial);
		}

		m_LayerList.insert(std::make_pair((UINT)m_LayerList.size(), renderData->m_TerrainBuffer));
	}
	break;
	default:
		break;
	}
	
	// Render Data 변환 및 등록..
	ConvertRenderData(originData, renderData);
}

void RenderDataConverter::ConvertRenderData(MeshData* originData, RenderData* renderData)
{
	// 해당 Instance Data 추출..
	MeshBuffer* originMesh = originData->Mesh_Buffer;
	MaterialBuffer* originMaterial = originData->Material_Buffer;

	// Mesh 혹은 Material 둘중 하나라도 없으면 Buffer 생성하지 않는다..
	if (originMesh == nullptr || originMaterial == nullptr) return;

	// 해당 Mesh & Material Buffer 설정..
	MeshRenderBuffer* convertMesh = GetMesh(originMesh->BufferIndex);
	MaterialRenderBuffer* convertMaterial = GetMaterial(originMaterial->BufferIndex);

	// Mesh & Material Buffer 기준 Instance 검색 및 Render Data 삽입..
	RegisterInstance(renderData, convertMesh, convertMaterial);
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
	// 현재 프레임 동안 쌓아둔 Push Mesh Queue 처리..
	while (!m_PushMeshList.empty())
	{
		// 현재 추가된 Mesh Buffer..
		MeshBuffer* mesh = m_PushMeshList.front();

		// 추가된 Resource 변환..
		ConvertPushMesh(mesh);

		// 변환된 Mesh Buffer Pop..
		m_PushMeshList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Push Material Queue 처리..
	while (!m_PushMaterialList.empty())
	{
		// 현재 추가된 Material Buffer..
		MaterialBuffer* material = m_PushMaterialList.front();

		// 추가된 Resource 변환..
		ConvertPushMaterial(material);

		// 변환된 Material Buffer Pop..
		m_PushMaterialList.pop();
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
		ConvertChangeMesh(mesh);

		// 변환된 Mesh Buffer Pop..
		m_ChangeMeshList.pop();
	}

	// 현재 프레임 동안 쌓아둔 Change Material Queue 처리..
	while (!m_ChangeMaterialList.empty())
	{
		// 현재 변동된 Resource가 있는 Material Buffer..
		MaterialBuffer* material = m_ChangeMaterialList.front();

		// 변동된 Resource 변환..
		ConvertChangeMaterial(material);

		// 변환된 Material Buffer Pop..
		m_ChangeMaterialList.pop();
	}
}

void RenderDataConverter::ConvertPushMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// 해당 Mesh가 재등록 되는 경우는 없어야한다..
	assert(itor == m_MeshList.end());

	// 새로운 Mesh Render Buffer 생성..
	MeshRenderBuffer* newMesh = new MeshRenderBuffer();
	newMesh->m_BufferIndex = meshIndex;

	// Mesh Render Buffer 변환..
	ConvertMesh(mesh, newMesh);

	// Mesh Render Buffer 삽입..
	m_MeshList.insert(std::pair<UINT, MeshRenderBuffer*>(meshIndex, newMesh));
}

void RenderDataConverter::ConvertPushMaterial(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// 해당 Index Material 체크..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// 해당 Material이 재등록 되는 경우는 없어야한다..
	assert(itor == m_MaterialList.end());

	// 새로운 Material Render Buffer 생성..
	MaterialRenderBuffer* newMaterial = new MaterialRenderBuffer();
	newMaterial->m_BufferIndex = materialIndex;

	// Material 기본 Data 설정..
	newMaterial->m_MaterialSubData = material->Material_SubData;

	// Material Render Buffer 변환..
	ConvertMaterial(material, newMaterial);

	// Material Render Buffer 삽입..
	m_MaterialList.insert(std::pair<UINT, MaterialRenderBuffer*>(materialIndex, newMaterial));
}

void RenderDataConverter::ConvertChangeMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// 해당 Mesh가 없을 경우는 없어야한다..
	assert(itor != m_MeshList.end());

	// Mesh Render Buffer 재설정..
	ConvertMesh(mesh, itor->second);
}

void RenderDataConverter::ConvertChangeMaterial(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// 해당 Index Material 체크..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// 해당 Material이 없을 경우는 없어야한다..
	assert(itor != m_MaterialList.end());

	// Material Render Buffer 재설정..
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

	// 해당 Material Data 변환..
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

	// 해당 Instance Buffer 삭제..
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

	// 해당 Instance Buffer 삭제..
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

	// Instance List에서 동일한 Instance Buffer 찾기..
	for (auto& instance : m_InstanceList)
	{
		// 현재 검색할 Instance Buffer..
		instanceBuffer = instance.second;

		// Mesh Buffer와 Material Buffer가 같을 경우 같은 Instance 취급..
		if (instanceBuffer->m_Mesh == mesh && instanceBuffer->m_Material == material)
		{
			// 해당 Instance Index 삽입..
			renderData->m_InstanceIndex = instanceBuffer->m_BufferIndex;
			renderData->m_InstanceLayerIndex = instanceBuffer->m_BufferIndex;

			// 해당 Instance Layer에 Render Data 삽입..
			instanceLayer = GetLayer(instanceBuffer->m_BufferIndex);
			instanceLayer->PushRenderData(renderData);

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
		instance_Index = m_InstanceIndexList.size();
		m_InstanceIndexList.push_back(std::pair<UINT, bool>(instance_Index, true));
	}

	// 새로운 Instance Buffer 생성..
	instanceBuffer = new InstanceRenderBuffer();

	// 현재 Instance Index 설정..
	instanceBuffer->m_BufferIndex = instance_Index;
	instanceBuffer->m_Type = (UINT)renderData->m_ObjectData->ObjType;
	instanceBuffer->m_Mesh = mesh;
	instanceBuffer->m_Material = material;

	// Instance List 추가..
	m_InstanceList.insert(std::make_pair(instance_Index, instanceBuffer));

	// 새로운 Instance Layer 생성..
	instanceLayer = new InstanceLayer();
	instanceLayer->m_LayerIndex = instance_Index;
	instanceLayer->m_Instance = instanceBuffer;
	instanceLayer->PushRenderData(renderData);

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
