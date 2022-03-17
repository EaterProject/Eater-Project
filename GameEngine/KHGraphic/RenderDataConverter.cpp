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
	// Origin Mesh Data 설정..
	renderData->m_OriginData = originData;

	// Mesh Data 설정..
	renderData->m_ObjectData = originData->Object_Data;
	renderData->m_ParticleData = originData->Particle_Data;
	renderData->m_ColliderData = originData->Collider_Data;

	// 해당 Mesh & Material Buffer 설정..
	renderData->m_MeshBuffer	 = FindMesh(originData->Mesh_Buffer->BufferIndex);
	renderData->m_MaterialBuffer = FindMaterial(originData->Material_Buffer->BufferIndex);

	// Obejct Type에 따른 추가 변환 작업..
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
			// 해당 Material 찾기..
			MaterialRenderBuffer* layerMaterial = FindMaterial(layer->BufferIndex);

			// Material List 추가..
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

	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// 해당 Mesh가 없을 경우는 없어야한다..
	assert(itor != m_MeshList.end());

	// Mesh Render Buffer 재설정..
	ConvertMeshData(mesh, itor->second);
}

void RenderDataConverter::ChangeMaterialBuffer(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// 해당 Index Material 체크..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// 해당 Material이 없을 경우는 없어야한다..
	assert(itor != m_MaterialList.end());

	// Material Render Buffer 재설정..
	ConvertMaterialData(material, itor->second);
}

void RenderDataConverter::PushMesh(MeshBuffer* mesh)
{
	// Mesh Index..
	UINT meshIndex = mesh->BufferIndex;

	// 해당 Index Mesh 체크..
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(meshIndex);

	// 해당 Mesh가 재등록 되는 경우는 없어야한다..
	assert(itor == m_MeshList.end());

	// 새로운 Mesh Render Buffer 생성..
	MeshRenderBuffer* newMesh = new MeshRenderBuffer();

	// Mesh Buffer Index 삽입..
	newMesh->m_BufferIndex = mesh->BufferIndex;

	// Mesh Render Buffer 변환..
	ConvertMeshData(mesh, newMesh);

	// Mesh Render Buffer 삽입..
	m_MeshList.insert(std::pair<UINT, MeshRenderBuffer*>(meshIndex, newMesh));
}

void RenderDataConverter::PushMaterial(MaterialBuffer* material)
{
	// Material Index..
	UINT materialIndex = material->BufferIndex;

	// 해당 Index Material 체크..
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(materialIndex);

	// 해당 Material이 재등록 되는 경우는 없어야한다..
	assert(itor == m_MaterialList.end());

	// 새로운 Material Render Buffer 생성..
	MaterialRenderBuffer* newMaterial = new MaterialRenderBuffer();

	// Material 기본 Data 설정..
	newMaterial->m_MaterialIndex = material->BufferIndex;
	newMaterial->m_MaterialSubData = material->Material_SubData;

	// Material Render Buffer 변환..
	ConvertMaterialData(material, newMaterial);

	// Material Render Buffer 삽입..
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
	// 해당 Material Data 변환..
	if (originMat->Albedo) convertMat->m_Albedo		= (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal		= (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM			= (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;

}
