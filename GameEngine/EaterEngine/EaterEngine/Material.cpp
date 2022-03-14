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

void Material::PushMaterialData(LoadMeshData* mesh)
{
	// Load�� Material Data�� �������..
	if (mesh->Material == nullptr) return;

	// Material Data ����..
	MaterialOption* matData = MaterialDatas->Material_Option;

	// Material Data ����..
	matData->Ambient = mesh->Material->m_Material_Ambient;
	matData->Diffuse = mesh->Material->m_Material_Diffuse;
	matData->Specular = mesh->Material->m_Material_Specular;
	matData->Ambient = mesh->Material->m_Material_Ambient;

	// Texture Map ����..
	//MaterialDatas->Albedo = mesh->Albedo;
	//MaterialDatas->Normal = mesh->Normal;
	//MaterialDatas->Emissive = mesh->Emissive;
	//MaterialDatas->ORM = mesh->ORM;
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