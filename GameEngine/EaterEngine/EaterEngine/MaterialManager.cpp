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
	Release();
}

void MaterialManager::Initialize()
{

}

void MaterialManager::Release()
{
	// Material List 삭제..
	for (auto& material : g_MaterialList)
	{
		SAFE_RELEASE(material.second);
	}

	g_MaterialList.clear();
	g_IndexList.clear();
}

void MaterialManager::PushMaterial(Material* material)
{
	// 추가된 Material Index 부여..
	UINT material_Index = 0;

	for (int i = 0; i < g_IndexList.size(); i++)
	{
		// Index List에 빈곳이 있다면 해당 Index 부여..
		if (g_IndexList[i].second == false)
		{
			material_Index = g_IndexList[i].first;
			g_IndexList[i].second = true;
			break;
		}
	}

	// 만약 Index List에 빈곳이 없다면 다음 Index 추가..
	if (material_Index == 0)
	{
		material_Index = g_IndexList.size();
		g_IndexList.push_back(std::make_pair(material_Index, true));
	}

	// 현재 Material Index 설정..
	material->m_MaterialData->BufferIndex = material_Index;
	
	// Material List 추가..
	g_MaterialList.insert(std::make_pair(material_Index, material));
}

void MaterialManager::DeleteMaterial(UINT index)
{
	// 해당 Material 검색..
	Material* material = g_MaterialList.find(index)->second;

	// 해당 Material Data 삭제..
	SAFE_RELEASE(material);
	g_MaterialList.erase(index);

	// 해당 Material Index 빈곳으로 설정..
	g_IndexList[index].second = false;
}
