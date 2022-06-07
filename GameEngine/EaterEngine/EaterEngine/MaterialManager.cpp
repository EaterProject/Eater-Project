#include "MaterialManager.h"
#include "Material.h"

std::map<Material*, bool> MaterialManager::g_MaterialPool;

MaterialManager::MaterialManager()
{
	// 미리 Default Material 생성..
	//for (int i = 0; i < 10; i++)
	//{
	//	g_MaterialPool.insert(std::pair<Material*, bool>(new Material(), false));
	//}
}

MaterialManager::~MaterialManager()
{

}

Material* MaterialManager::CreateDefaultMaterial()
{
	// 생성되 있는 Material 중 사용하지 않는 Material 부여..
	for (auto& material : g_MaterialPool)
	{
		if (material.second == false)
		{
			material.second = true;
			return material.first;
		}
	}

	// 만약 모든 Material이 사용중이라면 새로운 Material 생성 및 부여..
	Material* newMaterial = new Material();
	newMaterial->Name = "Defalt_" + std::to_string(g_MaterialPool.size());
	newMaterial->m_MaterialData->Name = newMaterial->Name;
	newMaterial->is_Default = true;
	
	g_MaterialPool.insert(std::pair<Material*, bool>(newMaterial, true));

	return newMaterial;
}

void MaterialManager::DestroyDefaultMaterial(Material* material)
{
	// 해당 Material 검색..
	const auto& itor = g_MaterialPool.find(material);

	if (itor != g_MaterialPool.end())
	{
		itor->second = false;
	}
}

void MaterialManager::Release()
{
	for (auto& material : g_MaterialPool)
	{
		delete material.first;
	}

	g_MaterialPool.clear();
}
