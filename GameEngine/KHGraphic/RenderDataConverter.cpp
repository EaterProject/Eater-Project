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

	// Mesh Data 설정..
	ConvertMeshBuffer(originData->Mesh_Buffer, renderData->m_MeshBuffer);

	// Material Data 설정..
	ConvertMaterial(originData->Material_Buffer, renderData->m_MaterialBuffer);

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
			// 새로운 Material 생성..
			MaterialRenderBuffer* layerMaterial = new MaterialRenderBuffer();

			// Material Data 변환..
			ConvertMaterial(layer, layerMaterial);

			// Material List 추가..
			renderData->m_TerrainData->m_MaterialList.push_back(layerMaterial);
		}
	}
	break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		// 해당 Particle System Data 삽입..
		renderData->m_ParticleData = originData->Particle_Data;
	}
	break;
	default:
		break;
	}
}

void RenderDataConverter::ChangeMeshData(UINT index)
{

}

void RenderDataConverter::ChangeMaterialData(UINT index)
{

}

MeshRenderBuffer* RenderDataConverter::GetMeshRenderData(UINT index)
{
	std::unordered_map<UINT, MeshRenderBuffer*>::iterator itor = m_MeshList.find(index);

	if (itor == m_MeshList.end()) return nullptr;

	return itor->second;
}

MaterialRenderBuffer* RenderDataConverter::GetMaterialRenderData(UINT index)
{
	std::unordered_map<UINT, MaterialRenderBuffer*>::iterator itor = m_MaterialList.find(index);

	if (itor == m_MaterialList.end()) return nullptr;

	return itor->second;
}

void RenderDataConverter::ConvertMeshBuffer(MeshBuffer* originBuf, MeshRenderBuffer* convertData)
{
	if (originBuf == nullptr) return;

	// Mesh Buffer Index 삽입..
	convertData->m_BufferIndex = originBuf->BufferIndex;

	// Index Buffer Data Convert..
	convertData->m_IndexCount = originBuf->IndexBuf->Count;
	convertData->m_IndexBuf = (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	convertData->m_Stride = originBuf->VertexBuf->Stride;
	convertData->m_VertexBuf = (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderDataConverter::ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat)
{
	// 해당 Material Data 변환..
	convertMat->m_MaterialIndex = originMat->Material_Index;
	convertMat->m_MaterialSubData = originMat->Material_SubData;

	if (originMat->Albedo) convertMat->m_Albedo = (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal = (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM = (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;

}
