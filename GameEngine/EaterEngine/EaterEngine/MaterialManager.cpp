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

	Global->Materials.push_back(mat);

	UINT material_Index = (UINT)g_MaterialList.size();

	// Material List �߰�..
	g_MaterialList.insert(std::make_pair(material_Index, mat));
}

void MaterialManager::Release()
{
	// Material List ����..
	g_MaterialList.clear();

	// Global Data Material Data ����..
	Global->Materials.clear();
}

void MaterialManager::PushMaterial(Material* mat)
{
	// Material �߰� �ɶ����� Global Data ����..
	MaterialData* matBuf = mat->MaterialDatas;

	UINT material_Index = (UINT)g_MaterialList.size();

	// ���� Material Index ����..
	matBuf->Material_Index = material_Index;
	
	// ���� Material Data ����..
	Global->Materials.push_back(matBuf->Material_Option);

	// Material List �߰�..
	g_MaterialList.insert(std::make_pair(material_Index, matBuf->Material_Option));
}

void MaterialManager::DeleteMaterial(UINT index)
{
	// �ش� Material Data ����..
	//g_MaterialList.erase(index);

	// Global Data Material Data ����..
	//Global->Materials.erase(std::next(Global->Materials.begin(), index));
}
