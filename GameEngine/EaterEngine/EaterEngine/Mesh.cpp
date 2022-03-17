#include "Mesh.h"
#include "EngineData.h"
#include "MeshManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer 생성..
	m_MeshData = new MeshBuffer();

	// Mesh 등록..
	MeshManager::PushMesh(this);
}

Mesh::~Mesh()
{
	Release();
}

void Mesh::Release()
{
	// Manager 내부에 있는 해당 Mesh Data 삭제..
	MeshManager::DeleteMesh(m_MeshData->BufferIndex);

	// 해당 Mesh Data 해제..
	SAFE_DELETE(m_MeshData);
}
