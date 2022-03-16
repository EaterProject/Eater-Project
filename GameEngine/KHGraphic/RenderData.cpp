#include "DirectDefine.h"
#include "EngineData.h"
#include "RenderData.h"

RenderData::RenderData()
{
	// Renderer 전용 Mesh Data 생성..
	m_MeshData = new MeshRenderData();
}

RenderData::~RenderData()
{
	Release();
}

void RenderData::ConvertData(MeshData* originMesh)
{
	// Origin Mesh Data 설정..
	m_OriginData = originMesh;

	// Object Type 설정..
	m_ObjectType = originMesh->ObjType;

	// Mesh Data 설정..
	m_World = originMesh->World;

	m_ColliderData = originMesh->Collider_Data;

	// Mesh Buffer Data 변환..
	ConvertMeshBuffer(originMesh->MeshBuf);

	// Material Data 변환..
	ConvertMaterial(originMesh->Material_Data);

	// Obejct Type에 따른 추가 변환 작업..
	switch (m_ObjectType)
	{
	case OBJECT_TYPE::SKINNING:
	{
		m_BoneOffsetTM = &originMesh->BoneOffsetTM;
	}
		break;
	case OBJECT_TYPE::TERRAIN:
	{
		m_TerrainData = new TerrainRenderData();

		for (MaterialData* layer : originMesh->Terrain_Data->Material_List)
		{
			// 새로운 Material 생성..
			MaterialRenderData* layerMaterial = new MaterialRenderData();

			// Material Data 변환..
			ConvertMaterial(layer, layerMaterial);

			// Material List 추가..
			m_TerrainData->m_MaterialList.push_back(layerMaterial);
		}
	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
	{
		// 해당 Particle System Data 삽입..
		m_ParticleData = originMesh->Particle_Data;
	}
		break;
	default:
		break;
	}
}

void RenderData::Release()
{
	for (MaterialRenderData* mat : m_TerrainData->m_MaterialList)
	{
		SAFE_DELETE(mat);
	}

	m_TerrainData->m_MaterialList.clear();

	SAFE_DELETE(m_TerrainData);
	SAFE_DELETE(m_MeshData->m_Material);
	SAFE_DELETE(m_MeshData);
}

void RenderData::ConvertMeshBuffer(MeshBuffer* originBuf)
{
	if (originBuf == nullptr) return;

	// Mesh Buffer Index 삽입..
	m_MeshData->m_BufferIndex = originBuf->BufferIndex;

	// Index Buffer Data Convert..
	m_MeshData->m_IndexCount = originBuf->IndexBuf->Count;
	m_MeshData->m_IndexBuf = (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	m_MeshData->m_Stride = originBuf->VertexBuf->Stride;
	m_MeshData->m_VertexBuf = (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderData::ConvertMaterial(MaterialData* originMat)
{
	if (originMat == nullptr) return;

	if (m_MeshData->m_Material == nullptr)
	{
		m_MeshData->m_Material = new MaterialRenderData();
	}

	ConvertMaterial(originMat, m_MeshData->m_Material);
}

void RenderData::ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat)
{
	// 해당 Material Data 변환..
	convertMat->m_MaterialIndex = originMat->Material_Index;
	convertMat->m_MaterialSubData = originMat->Material_SubData;

	if (originMat->Albedo) convertMat->m_Albedo		= (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal		= (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive	= (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM			= (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;
}
