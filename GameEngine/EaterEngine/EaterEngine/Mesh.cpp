#include "Mesh.h"
#include "EngineData.h"
#include "MeshManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer 생성..
	m_MeshData = new MeshBuffer();
	m_MeshData->Name = "None";

	// Mesh 등록..
	MeshManager::PushMesh(this);

	// Mesh Graphic 측 등록..
	GraphicEngine::Get()->PushMesh(m_MeshData);
}

Mesh::~Mesh()
{
	Release();
}

void Mesh::Release()
{
	// Manager 내부에 있는 해당 Mesh Data 삭제..
	MeshManager::DeleteMesh(m_MeshData->BufferIndex);

	// Graphic 내부에 있는 해당 Mesh Buffer 삭제..
	GraphicEngine::Get()->DeleteMesh(m_MeshData);

	// 해당 Mesh Data 해제..
	SAFE_DELETE(m_MeshData);
}
