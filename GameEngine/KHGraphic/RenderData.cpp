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

	// Mesh Index 설정..
	m_MeshIndex = originMesh->MeshIndex;

	// Alpha Mesh 여부 설정..
	m_Alpha = originMesh->Alpha;

	// Mesh Data 설정..
	m_World = originMesh->mWorld;
	m_Tex	= originMesh->mTexTM;

	m_ColliderData = originMesh->Collider_Data;

	// Index Buffer Data 설정..
	ConvertIndexBuffer(originMesh->IndexBuf);

	// Vertex Buffer Data 설정..
	ConvertVertexBuffer(originMesh->VertexBuf);

	// Material Data 설정..
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
		for (MaterialData* layer : originMesh->Terrain_Data->Material_List)
		{
			// 새로운 Material 생성..
			MaterialRenderData* layerMaterial = new MaterialRenderData();

			// Material Data 변환..
			ConvertMaterial(layer, layerMaterial);

			// Material List 추가..
			m_MeshData->m_MaterialList.push_back(layerMaterial);
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
	for (MaterialRenderData* mat : m_MeshData->m_MaterialList)
	{
		SAFE_DELETE(mat);
	}

	m_MeshData->m_MaterialList.clear();

	SAFE_DELETE(m_MeshData->m_Material);
	SAFE_DELETE(m_MeshData);
}

void RenderData::ConvertIndexBuffer(IndexBuffer* originBuf)
{
	if (originBuf == nullptr) return;

	m_MeshData->m_IndexCount = originBuf->Count;
	m_MeshData->m_IndexBuf	 = (ID3D11Buffer*)originBuf->pIndexBuf;
}

void RenderData::ConvertVertexBuffer(VertexBuffer* originBuf)
{
	if (originBuf == nullptr) return;

	m_MeshData->m_Stride	= originBuf->Stride;
	m_MeshData->m_VertexBuf = (ID3D11Buffer*)originBuf->pVertexBuf;
}

void RenderData::ConvertMaterial(MaterialData* originMat)
{
	if (originMat == nullptr) return;

	m_MeshData->m_Material = new MaterialRenderData();

	ConvertMaterial(originMat, m_MeshData->m_Material);
}

void RenderData::ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat)
{
	// 해당 Material Data 변환..
	convertMat->m_MaterialIndex = originMat->Material_Index;

	if (originMat->Albedo) convertMat->m_Albedo		= (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal		= (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive	= (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM			= (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;
}
