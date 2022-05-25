#pragma once
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "EngineData.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

class IndexManager
{
public:
	IndexManager();
	virtual ~IndexManager();

public:
	static void Release();

	static void SetIndex(UINT* index);
	static void DeleteIndex(UINT index);

private:
	static std::vector<std::pair<UINT, bool>> g_IndexList;
};