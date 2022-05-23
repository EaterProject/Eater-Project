#pragma once
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "EngineData.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

template<typename T>
class IndexManager
{
public:
	IndexManager();
	~IndexManager();

public:
	static void Release();

	static void PushResource(T* resource, UINT* index);
	static void DeleteResource(UINT index);

private:
	static std::vector<std::pair<UINT, bool>> g_IndexList;
	static std::unordered_map<UINT, T*> g_ResourceList;
};

template<typename T>
std::vector<std::pair<UINT, bool>> IndexManager<T>::g_IndexList;

template<typename T>
std::unordered_map<UINT, T*> IndexManager<T>::g_ResourceList;

template<typename T>
inline IndexManager<T>::IndexManager()
{

}

template<typename T>
inline IndexManager<T>::~IndexManager()
{
	Release();
}

template<typename T>
inline void IndexManager<T>::Release()
{
	// Resource List ����..
	for (auto& resource : g_ResourceList)
	{
		SAFE_RELEASE(resource.second);
	}

	g_ResourceList.clear();
	g_IndexList.clear();
}

template<typename T>
inline void IndexManager<T>::PushResource(T* resource, UINT* index)
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

	// Mesh List �߰�..
	g_ResourceList.insert(std::make_pair(resource_Index, resource));
}

template<typename T>
inline void IndexManager<T>::DeleteResource(UINT index)
{
	// �ش� Resource �˻�..
	T* resource = g_ResourceList.find(index)->second;

	// �ش� Resource Data ����..
	g_ResourceList.erase(index);

	// �ش� Resource Index ������� ����..
	g_IndexList[index].second = false;
}
