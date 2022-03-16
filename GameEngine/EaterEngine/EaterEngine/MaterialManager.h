#pragma once
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "BaseManager.h"

class Material;
class MaterialManager : public BaseManager
{
public:
	MaterialManager();
	~MaterialManager();

public:
	void Initialize();
	void Release();

	static void PushMaterial(Material* material);
	static void DeleteMaterial(UINT index);

private:
	static std::vector<std::pair<UINT, bool>> g_IndexList;
	static std::unordered_map<UINT, Material*> g_MaterialList;
};
