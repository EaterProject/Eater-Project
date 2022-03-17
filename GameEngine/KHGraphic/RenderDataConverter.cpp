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

void RenderDataConverter::ConvertRenderData(MeshData* originData, RenderData* renderData)
{
	// Origin Mesh Data ����..
	renderData->m_OriginData = originData;

	// Mesh Data ����..
	renderData->m_ObjectData = originData->Object_Data;
	renderData->m_ParticleData = originData->Particle_Data;
	renderData->m_ColliderData = originData->Collider_Data;

	// �ش� Mesh & Material Buffer ����..
	renderData->m_MeshBuffer	 = FindMesh(originData->Mesh_Buffer->BufferIndex);
	renderData->m_MaterialBuffer = FindMaterial(originData->Material_Buffer->BufferIndex);

	// Obejct Type�� ���� �߰� ��ȯ �۾�..
	switch (renderData->m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::SKINNING:
	{
		renderData->m_BoneOffsetTM = &originData->BoneOffsetTM;
	}
	break;
	case OBJECT_TYPE::TERRAIN:
	{
		renderData->m_TerrainData = new TerrainRenderData();

		for (MaterialBuffer* layer : originData->Terrain_Data->Material_List)
		{
			// �ش� Material ã��..
			MaterialRenderBuffer* layerMaterial = FindMaterial(layer->BufferIndex);

			// Material List �߰�..
			renderData->m_TerrainData->m_MaterialList.push_back(layerMaterial);
		}
	}
	break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	break;
	default:
		break;
	}
}

void RenderDataConverter::ChangeMeshBuffer(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// �ش� Mesh�� ���� ���� ������Ѵ�..
	assert(itor != m_MeshList.end());

	// Mesh Render Buffer �缳��..
	ConvertMeshData(mesh, itor->second);
}

void RenderDataConverter::ChangeMaterialBuffer(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// �ش� Index Material üũ..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// �ش� Material�� ���� ���� ������Ѵ�..
	assert(itor != m_MaterialList.end());

	// Material Render Buffer �缳��..
	ConvertMaterialData(material, itor->second);
}

void RenderDataConverter::PushMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// �ش� Index Mesh üũ..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// �ش� Mesh�� ���� �Ǵ� ���� ������Ѵ�..
	assert(itor == m_MeshList.end());

	// ���ο� Mesh Render Buffer ����..
	MeshRenderBuffer* newMesh = new MeshRenderBuffer();

	// Mesh Buffer Index ����..
	newMesh->m_BufferIndex = mesh->BufferIndex;

	// Mesh Render Buffer ��ȯ..
	ConvertMeshData(mesh, newMesh);

	// Mesh Render Buffer ����..
	m_MeshList.insert(std::pair<UINT, MeshRenderBuffer*>(meshIndex, newMesh));
}

void RenderDataConverter::PushMaterial(MaterialBuffer* material)
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
	newMaterial->m_MaterialIndex = material->BufferIndex;
	newMaterial->m_MaterialSubData = material->Material_SubData;

	// Material Render Buffer ��ȯ..
	ConvertMaterialData(material, newMaterial);

	// Material Render Buffer ����..
	m_MaterialList.insert(std::pair<UINT, MaterialRenderBuffer*>(materialIndex, newMaterial));
}

MeshRenderBuffer* RenderDataConverter::FindMesh(UINT index)
{
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	if(itor == m_MeshList.end()) return;

	return itor->second;
}

MaterialRenderBuffer* RenderDataConverter::FindMaterial(UINT index)
{
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(index);

	if(itor == m_MaterialList.end()) return nullptr;

	return itor->second;
}

InstanceRenderBuffer* RenderDataConverter::FindInstance(UINT index)
{
	std::unordered_map<UINT, InstanceRenderBuffer*>::iterator itor = m_InstanceList.find(index);

	if (itor == m_InstanceList.end()) return nullptr;

	return itor->second;
}

void RenderDataConverter::ConvertMeshData(MeshBuffer* originBuf, MeshRenderBuffer* convertData)
{
	if (originBuf == nullptr) return;

	// Index Buffer Data Convert..
	convertData->m_IndexCount	= originBuf->IndexBuf->Count;
	convertData->m_IndexBuf		= (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	convertData->m_Stride		= originBuf->VertexBuf->Stride;
	convertData->m_VertexBuf	= (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderDataConverter::ConvertMaterialData(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat)
{
	// �ش� Material Data ��ȯ..
	if (originMat->Albedo) convertMat->m_Albedo		= (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal		= (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM			= (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;

}
