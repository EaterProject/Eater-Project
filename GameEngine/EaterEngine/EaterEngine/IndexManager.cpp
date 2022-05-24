#include "IndexManager.h"

std::vector<std::pair<UINT, bool>> IndexManager::g_IndexList;

IndexManager::IndexManager()
{

}

IndexManager::~IndexManager()
{

}

void IndexManager::Release()
{
	g_IndexList.clear();
}

void IndexManager::SetIndex(UINT* index)
{
	// �߰��� Resource Index �ο�..
	UINT resource_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List�� ����� �ִٸ� �ش� Index �ο�..
		if (g_IndexList[i].second == false)
		{
			resource_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// ���� Index List�� ����� ���ٸ� ���� Index �߰�..
	if (resource_Index == 0)
	{
		resource_Index = (UINT)g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(resource_Index, true));
	}

	// ���� Resource Index ����..
	*index = resource_Index;
}

void IndexManager::DeleteIndex(UINT index)
{
	// �ش� Resource Index ������� ����..
	g_IndexList[index].second = false;
}
