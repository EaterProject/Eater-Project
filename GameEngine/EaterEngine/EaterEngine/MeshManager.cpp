#include "MeshManager.h"
#include "Mesh.h"
#include "EngineData.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

std::vector<std::pair<UINT, bool>> MeshManager::g_IndexList;
std::unordered_map<UINT, Mesh*> MeshManager::g_MeshList;

MeshManager::MeshManager()
{

}

MeshManager::~MeshManager()
{
	Release();
}

void MeshManager::Initialize()
{

}

void MeshManager::Release()
{
	// Material List 삭제..
	for (auto& mesh : g_MeshList)
	{
		SAFE_RELEASE(mesh.second);
	}

	g_MeshList.clear();
	g_IndexList.clear();
}

void MeshManager::PushMesh(Mesh* mesh)
{
	// 추가된 Mesh Index 부여..
	UINT mesh_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (g_IndexList[i].second == false)
		{
			mesh_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (mesh_Index == 0)
	{
		mesh_Index = g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(mesh_Index, true));
	}

	// 현재 Mesh Index 설정..
	mesh->m_MeshData->BufferIndex = mesh_Index;

	// Mesh List 추가..
	g_MeshList.insert(std::make_pair(mesh_Index, mesh));
}

void MeshManager::DeleteMesh(UINT index)
{
	// 해당 Mesh 검색..
	Mesh* mesh = g_MeshList.find(index)->second;

	// 해당 Mesh Data 삭제..
	SAFE_RELEASE(mesh);
	g_MeshList.erase(index);

	// 해당 Mesh Index 빈곳으로 설정..
	g_IndexList[index].second = false;
}