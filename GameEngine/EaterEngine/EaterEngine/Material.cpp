#include "Material.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "MaterialManager.h"
#include "LoadManager.h"


Material::Material()
{
	// Material Buffer ����..
	MaterialDatas = new MaterialData();

	// Material Data ����..
	MaterialDatas->Material_Option = new MaterialOption();

	// Material ���..
	MaterialManager::PushMaterial(this);
}

Material::~Material()
{
	Release();
}

void Material::SetMeshData(MeshData* meshData)
{
	// �ش� Object Material Data ����..
	meshData->Material_Data = MaterialDatas;

	m_MeshData = meshData;
}

void Material::SetMaterialIndex(UINT index)
{
	// �ش� Material Index ����..
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
	// Manager ���ο� �ִ� �ش� Material Data ����..
	MaterialManager::DeleteMaterial(MaterialDatas->Material_Index);
	
	// �ش� Material Data ����..
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