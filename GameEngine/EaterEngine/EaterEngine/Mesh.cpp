#include "Mesh.h"
#include "EngineData.h"
#include "MeshManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer ����..
	m_MeshData = new MeshBuffer();
	m_MeshData->Name = "None";

	m_MeshData->Mesh_Property = new MeshProperty();

	// Mesh ���..
	MeshManager::PushMesh(this);

	// Mesh Graphic �� ���..
	GraphicEngine::Get()->PushMesh(m_MeshData);
}

Mesh::~Mesh()
{
	// Manager ���ο� �ִ� �ش� Mesh Data ����..
	MeshManager::DeleteMesh(m_MeshData->BufferIndex);
}

void Mesh::Release()
{
	// Graphic ���ο� �ִ� �ش� Mesh Buffer ����..
	GraphicEngine::Get()->DeleteMesh(m_MeshData);

	// �ش� Mesh Data ����..
	SAFE_DELETE(m_MeshData);
}
