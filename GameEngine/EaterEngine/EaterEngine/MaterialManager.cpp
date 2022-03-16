#include "MaterialManager.h"
#include "Material.h"
#include "EngineData.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

std::vector<std::pair<UINT, bool>> MaterialManager::g_IndexList;
std::unordered_map<UINT, Material*> MaterialManager::g_MaterialList;

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::Initialize()
{

}

void MaterialManager::Release()
{
	// Material List ����..
	for (auto& material : g_MaterialList)
	{
		SAFE_RELEASE(material.second);
	}

	g_MaterialList.clear();
}

void MaterialManager::PushMaterial(Material* material)
{
	// �߰��� Material Index �ο�..
	UINT material_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List�� ����� �ִٸ� �ش� Index �ο�..
		if (g_IndexList[i].second == false)
		{
			material_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
		}
	}

	// ���� Index List�� ����� ���ٸ� ���� Index �߰�..
	if (material_Index == 0)
	{
		material_Index = g_IndexList.size();
		g_IndexList.push_back(std::pair<UINT, bool>(material_Index++, true));
	}

	// ���� Material Index ����..
	material->m_MaterialData->Material_Index = material_Index;
	
	// Material List �߰�..
	g_MaterialList.insert(std::make_pair(material_Index, material));
}

void MaterialManager::DeleteMaterial(UINT index)
{
	// �ش� Material Data ����..
	g_MaterialList.erase(index);

	// �ش� Material Index ������� ����..
	g_IndexList[index].second = false;
}
