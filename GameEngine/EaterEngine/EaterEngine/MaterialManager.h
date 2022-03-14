#pragma once
#include <unordered_map>
#include "LightHelper.h"

class Material;
class MaterialManager : public BaseManager
{
public:
	MaterialManager();
	~MaterialManager();

public:
	void Initialize();
	void Release();

	static void PushMaterial(Material* mat);
	static void DeleteMaterial(UINT index);

private:
	static std::unordered_map<UINT, MaterialOption*> g_MaterialList;
};

