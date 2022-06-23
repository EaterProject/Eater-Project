#include "Mesh.h"
#include "EngineData.h"
#include "GameEngine.h"
#include "GraphicEngineAPI.h"
#include "IndexManager.h"
#include "MeshManager.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Mesh::Mesh()
{
	// Mesh Buffer 생성..
	m_MeshData = new MeshBuffer();
	m_MeshData->Name = "None";

	m_MeshData->Mesh_Property = new MeshProperty();

	// Mesh 등록..
	MeshManager::SetIndex(&m_MeshData->BufferIndex);

	// Mesh Graphic 측 등록..
	EnterCriticalSection(&GameEngine::g_CS);
	GraphicEngine::Get()->PushMesh(m_MeshData);
	LeaveCriticalSection(&GameEngine::g_CS);
}

Mesh::~Mesh()
{
	// Manager 내부에 있는 해당 Mesh Data 삭제..
	MeshManager::DeleteIndex(m_MeshData->BufferIndex);

	// 해당 Resource 제거..
	Release();
}

void Mesh::Release()
{
	// Graphic 내부에 있는 해당 Mesh Buffer 삭제..
	EnterCriticalSection(&GameEngine::g_CS);
	GraphicEngine::Get()->DeleteMesh(m_MeshData);
	LeaveCriticalSection(&GameEngine::g_CS); 

	// 해당 Mesh Data 해제..
	SAFE_DELETE(m_MeshData);
}
