#include "Mesh.h"
#include "EngineData.h"
#include "GameEngine.h"
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
	EnterCriticalSection(&GameEngine::g_CS);
	GraphicEngine::Get()->PushMesh(m_MeshData);
	LeaveCriticalSection(&GameEngine::g_CS);
}

Mesh::~Mesh()
{
	// Manager ���ο� �ִ� �ش� Mesh Data ����..
	MeshManager::DeleteIndex(m_MeshData->BufferIndex);

	// �ش� Resource ����..
	Release();
}

void Mesh::Release()
{
	// Graphic ���ο� �ִ� �ش� Mesh Buffer ����..
	EnterCriticalSection(&GameEngine::g_CS);
	GraphicEngine::Get()->DeleteMesh(m_MeshData);
	LeaveCriticalSection(&GameEngine::g_CS); 

	// �ش� Mesh Data ����..
	SAFE_DELETE(m_MeshData);
}
