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

	// Object Type 설정..
	renderData->m_ObjectType = originData->ObjType;

	// Mesh Index 설정..
	//renderData->m_MeshIndex = originData->MeshIndex;

	// Mesh Data 설정..
	renderData->m_World = originData->World;

	renderData->m_ColliderData = originData->Collider_Data;

	// Index Buffer Data 설정..
	//ConvertIndexBuffer(originData->IndexBuf, renderData->m_MeshData);

	// Vertex Buffer Data 설정..
	//ConvertVertexBuffer(originData->VertexBuf, renderData->m_MeshData);

	// Material Data 설정..
	ConvertMaterial(originData->Material_Data, renderData->m_MeshData);

	// Obejct Type에 따른 추가 변환 작업..
	switch (originData->ObjType)
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
			// 새로운 Material 생성..
			MaterialRenderData* layerMaterial = new MaterialRenderData();

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

void RenderDataConverter::ConvertMeshBuffer(MeshBuffer* originBuf, MeshRenderData* convertData)
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

void RenderDataConverter::ConvertMaterial(MaterialData* originMat, MeshRenderData* convertData)
{
	if (originMat == nullptr) return;

	if (convertData->m_Material == nullptr)
	{
		convertData->m_Material = new MaterialRenderData();
	}

	ConvertMaterial(originMat, convertData->m_Material);
}

void RenderDataConverter::ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat)
{
	// 해당 Material Data 변환..
	convertMat->m_MaterialIndex = originMat->Material_Index;
	convertMat->m_MaterialSubData = originMat->Material_SubData;

	if (originMat->Albedo) convertMat->m_Albedo = (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal = (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive = (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM = (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;

}
