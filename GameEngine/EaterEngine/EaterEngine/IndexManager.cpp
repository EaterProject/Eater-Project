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
	// 추가된 Resource Index 부여..
	UINT resource_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (g_IndexList[i].second == false)
		{
			resource_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (resource_Index == 0)
	{
		resource_Index = (UINT)g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(resource_Index, true));
	}

	// 현재 Resource Index 설정..
	*index = resource_Index;
}

void IndexManager::DeleteIndex(UINT index)
{
	// 해당 Resource Index 빈곳으로 설정..
	g_IndexList[index].second = false;
}
