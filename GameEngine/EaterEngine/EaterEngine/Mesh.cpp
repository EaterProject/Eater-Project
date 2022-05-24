#include "Mesh.h"
#include "EngineData.h"
#include "GraphicEngineAPI.h"
#include "IndexManager.h"
#include "MeshManager.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer ����..
	m_MeshData = new MeshBuffer();
	m_MeshData->Name = "None";

	m_MeshData->Mesh_Property = new MeshProperty();

	// Mesh ���..
	MeshManager::SetIndex(&m_MeshData->BufferIndex);

	// Mesh Graphic �� ���..
	GraphicEngine::Get()->PushMesh(m_MeshData);
}

Mesh::~Mesh()
{
	// �ش� Resource ����..
	Release();

	// Manager ���ο� �ִ� �ش� Mesh Data ����..
	MeshManager::DeleteIndex(m_MeshData->BufferIndex);
}

void Mesh::Release()
{
	// Graphic ���ο� �ִ� �ش� Mesh Buffer ����..
	GraphicEngine::Get()->DeleteMesh(m_MeshData);

	// �ش� Mesh Data ����..
	SAFE_DELETE(m_MeshData);
}
