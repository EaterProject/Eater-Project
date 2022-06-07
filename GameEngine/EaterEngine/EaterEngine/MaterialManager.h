#pragma once
#include "IndexManager.h"

class Material;

class MaterialManager : public IndexManager
{
public:
	MaterialManager();
	~MaterialManager();

public:
	static Material* CreateDefaultMaterial();
	static void DestroyDefaultMaterial(Material* material);

public:
	static void Release();

private:
	static std::map<Material*, bool> g_MaterialPool;
};

