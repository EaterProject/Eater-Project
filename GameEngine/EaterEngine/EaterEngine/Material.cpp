#include "Material.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "MaterialManager.h"
#include "LoadManager.h"


Material::Material()
{
	// Material Buffer 생성..
	MaterialDatas = new MaterialData();

	// Material Data 생성..
	MaterialDatas->Material_Option = new MaterialOption();

	// Material 등록..
	MaterialManager::PushMaterial(this);
}

Material::~Material()
{
	Release();
}

void Material::SetMeshData(MeshData* meshData)
{
	// 해당 Object Material Data 삽입..
	meshData->Material_Data = MaterialDatas;

	m_MeshData = meshData;
}

void Material::SetMaterialIndex(UINT index)
{
	// 해당 Material Index 삽입..
	MaterialDatas->Material_Index = index;
}

void Material::SetTexTransform(DirectX::SimpleMath::Vector3 scale)
{

}

void Material::SetTexTransform(float x, float y, float z)
{

}

void Material::SetDiffuseMap(std::string diffuseName)
{
	MaterialDatas->Albedo = LoadManager::GetTexture(diffuseName);
}

void Material::SetNormalMap(std::string noramlName)
{
	MaterialDatas->Albedo = LoadManager::GetTexture(noramlName);
}

void Material::SetBaseColor(DirectX::SimpleMath::Vector4 color)
{
	MaterialDatas->Color_Base = color;
}

void Material::SetStartColor(DirectX::SimpleMath::Vector4 color)
{
	MaterialDatas->Color_Add = color;
}

void Material::SetEmissiveFactor(float emissiveFactor)
{
	MaterialDatas->Material_Option->EmissiveFactor = emissiveFactor;
}

void Material::SetRoughnessFactor(float roughnessFactor)
{
	MaterialDatas->Material_Option->RoughnessFactor = roughnessFactor;
}

void Material::SetMetallicFactor(float metallicFactor)
{
	MaterialDatas->Material_Option->MetallicFactor = metallicFactor;
}

void Material::Release()
{
	// Manager 내부에 있는 해당 Material Data 삭제..
	MaterialManager::DeleteMaterial(MaterialDatas->Material_Index);
	
	// 해당 Material Data 해제..
	delete MaterialDatas->Material_Option;
	delete MaterialDatas;
}

MaterialData* Material::GetMaterialData()
{
	return MaterialDatas;
}

UINT Material::GetMaterialIndex()
{
	return MaterialDatas->Material_Index;
}