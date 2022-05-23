#include "Mesh.h"
#include "EngineData.h"
#include "GraphicEngineAPI.h"
#include "IndexManager.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer 생성..
	m_MeshData = new MeshBuffer();
	m_MeshData->Name = "None";

	m_MeshData->Mesh_Property = new MeshProperty();

	// Mesh 등록..
	IndexManager<MeshBuffer>::PushResource(m_MeshData, &m_MeshData->BufferIndex);

	// Mesh Graphic 측 등록..
	GraphicEngine::Get()->PushMesh(m_MeshData);
}

Mesh::~Mesh()
{
	// 해당 Resource 제거..
	Release();

	// Manager 내부에 있는 해당 Mesh Data 삭제..
	IndexManager<MeshBuffer>::DeleteResource(m_MeshData->BufferIndex);
}

void Mesh::Release()
{
	// Graphic 내부에 있는 해당 Mesh Buffer 삭제..
	GraphicEngine::Get()->DeleteMesh(m_MeshData);

	// 해당 Mesh Data 해제..
	SAFE_DELETE(m_MeshData);
}
