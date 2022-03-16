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

	// Mesh Data ����..
	ConvertMeshBuffer(originData->MeshBuffer_Data, renderData->m_MeshData);

	// Material Data ����..
	ConvertMaterial(originData->Material_Data, renderData->m_Material);

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

		for (MaterialData* layer : originData->Terrain_Data->Material_List)
		{
			// ���ο� Material ����..
			MaterialRenderData* layerMaterial = new MaterialRenderData();

			// Material Data ��ȯ..
			ConvertMaterial(layer, layerMaterial);

			// Material List �߰�..
			renderData->m_TerrainData->m_MaterialList.push_back(layerMaterial);
		}
	}
	break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		// �ش� Particle System Data ����..
		renderData->m_ParticleData = originData->Particle_Data;
	}
	break;
	default:
		break;
	}
}

void RenderDataConverter::ConvertMeshBuffer(MeshBuffer* originBuf, MeshRenderData* convertData)
{
	if (originBuf == nullptr) return;

	// Mesh Buffer Index ����..
	convertData->m_BufferIndex = originBuf->BufferIndex;

	// Index Buffer Data Convert..
	convertData->m_IndexCount = originBuf->IndexBuf->Count;
	convertData->m_IndexBuf = (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	convertData->m_Stride = originBuf->VertexBuf->Stride;
	convertData->m_VertexBuf = (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderDataConverter::ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat)
{
	// �ش� Material Data ��ȯ..
	convertMat->m_MaterialIndex = originMat->Material_Index;
	convertMat->m_MaterialSubData = originMat->Material_SubData;

	if (originMat->Albedo) convertMat->m_Albedo = (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal = (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM = (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;

}
