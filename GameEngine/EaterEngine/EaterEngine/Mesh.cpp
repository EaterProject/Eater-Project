#include "Mesh.h"
#include "EngineData.h"
#include "MeshManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer ����..
	m_MeshData = new MeshBuffer();

	// Mesh ���..
	MeshManager::PushMesh(this);
}

Mesh::~Mesh()
{
	Release();
}

void Mesh::Release()
{
	// Manager ���ο� �ִ� �ش� Mesh Data ����..
	MeshManager::DeleteMesh(m_MeshData->BufferIndex);

	// �ش� Mesh Data ����..
	SAFE_DELETE(m_MeshData);
}
