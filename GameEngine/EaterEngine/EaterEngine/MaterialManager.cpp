#include "BaseManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "EngineData.h"

std::unordered_map<UINT, MaterialOption*> MaterialManager::g_MaterialList;

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::Initialize()
{
	// Basic Material..
	MaterialOption* mat = new MaterialOption();

	Global->mMaterials.push_back(mat);

	UINT material_Index = (UINT)g_MaterialList.size();

	// Material List 추가..
	g_MaterialList.insert(std::make_pair(material_Index, mat));
}

void MaterialManager::Release()
{
	// Material List 삭제..
	g_MaterialList.clear();

	// Global Data Material Data 삭제..
	Global->mMaterials.clear();
}

void MaterialManager::PushMaterial(Material* mat)
{
	// 동일한 Material 체크
	for (auto& matData : g_MaterialList)
	{
		MaterialOption* material = matData.second;

		// 해당 Material Data가 List에 올라가있는지 체크..
		if (*material == *mat->MaterialDatas->Material_Option)
		{
			mat->SetMaterialIndex(matData.first);
			return;
		}
	}

	// Material 추가 될때마다 Global Data 설정..
	MaterialData* matBuf = mat->MaterialDatas;

	UINT material_Index = (UINT)g_MaterialList.size();

	// 현재 Material Index 설정..
	matBuf->Material_Index = material_Index;
	
	// 현재 Material Data 삽입..
	Global->mMaterials.push_back(matBuf->Material_Option);

	// Material List 추가..
	g_MaterialList.insert(std::make_pair(material_Index, matBuf->Material_Option));
}

void MaterialManager::DeleteMaterial(UINT index)
{
	// 해당 Material Data 삭제..
	//g_MaterialList.erase(index);

	// Global Data Material Data 삭제..
	//Global->mMatData.erase(std::next(Global->mMatData.begin(), index));
}
