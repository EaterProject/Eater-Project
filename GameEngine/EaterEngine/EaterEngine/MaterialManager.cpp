#include "MaterialManager.h"
#include "Material.h"

std::map<Material*, bool> MaterialManager::g_MaterialPool;

MaterialManager::MaterialManager()
{
	// �̸� Default Material ����..
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
	// ������ �ִ� Material �� ������� �ʴ� Material �ο�..
	for (auto& material : g_MaterialPool)
	{
		if (material.second == false)
		{
			material.second = true;
			return material.first;
		}
	}

	// ���� ��� Material�� ������̶�� ���ο� Material ���� �� �ο�..
	Material* newMaterial = new Material();
	newMaterial->Name = "Defalt_" + std::to_string(g_MaterialPool.size());
	newMaterial->m_MaterialData->Name = newMaterial->Name;
	newMaterial->is_Default = true;
	
	g_MaterialPool.insert(std::pair<Material*, bool>(newMaterial, true));

	return newMaterial;
}

void MaterialManager::DestroyDefaultMaterial(Material* material)
{
	// �ش� Material �˻�..
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
