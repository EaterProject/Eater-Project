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
	// Material List ����..
	for (auto& mesh : g_MeshList)
	{
		SAFE_RELEASE(mesh.second);
	}

	g_MeshList.clear();
	g_IndexList.clear();
}

void MeshManager::PushMesh(Mesh* mesh)
{
	// �߰��� Mesh Index �ο�..
	UINT mesh_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List�� ����� �ִٸ� �ش� Index �ο�..
		if (g_IndexList[i].second == false)
		{
			mesh_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// ���� Index List�� ����� ���ٸ� ���� Index �߰�..
	if (mesh_Index == 0)
	{
		mesh_Index = g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(mesh_Index, true));
	}

	// ���� Mesh Index ����..
	mesh->m_MeshData->BufferIndex = mesh_Index;

	// Mesh List �߰�..
	g_MeshList.insert(std::make_pair(mesh_Index, mesh));
}

void MeshManager::DeleteMesh(UINT index)
{
	// �ش� Mesh �˻�..
	Mesh* mesh = g_MeshList.find(index)->second;

	// �ش� Mesh Data ����..
	SAFE_RELEASE(mesh);
	g_MeshList.erase(index);

	// �ش� Mesh Index ������� ����..
	g_IndexList[index].second = false;
}