#include "Material.h"
#include "EngineData.h"
#include "BaseManager.h"
#include "MaterialManager.h"
#include "LoadManager.h"
#include "GraphicsEngine.h"

Material::Material()
{
	// Material Buffer 생성..
	m_MaterialData = new MaterialData();

	// Material Data 생성..
	m_MaterialData->Material_SubData = new MaterialSubData();

	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// Material 등록..
	MaterialManager::PushMaterial(this);
}

Material::~Material()
{
	Release();
}

void Material::SetTextureTiling(float scale_x, float scale_y)
{
	// 설정 Tiling에 따른 Texture Transform 설정..
	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f / scale_x, 1.0f / scale_y, 1.0f);
}

void Material::SetDiffuseTexture(std::string diffuseName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(diffuseName);

	// 해당 Texture가 없는 경우..
	if (newTexture == nullptr) return;

	// Texture 최초 설정이 아닌 변경일 경우 Renderer Data 동기화..
	if (m_MaterialData->Albedo != nullptr)
	{
		GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
	}

	// Texture 변경..
	m_MaterialData->Albedo = newTexture;
	m_MaterialData->Albedo->Name = diffuseName;
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);

	// 해당 Texture가 없는 경우..
	if (newTexture == nullptr) return;

	// Texture 최초 설정이 아닌 변경일 경우 Renderer Data 동기화..
	if (m_MaterialData->Normal != nullptr)
	{
		GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
	}

	// Texture 변경..
	m_MaterialData->Normal = newTexture;
	m_MaterialData->Normal->Name = noramlName;
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// 해당 Texture가 없는 경우..
	if (newTexture == nullptr) return;

	// Texture 최초 설정이 아닌 변경일 경우 Renderer Data 동기화..
	if (m_MaterialData->Emissive != nullptr)
	{
		GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
	}

	// Texture 변경..
	m_MaterialData->Emissive = newTexture;
	m_MaterialData->Emissive->Name = emissiveName;
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// 해당 Texture가 없는 경우..
	if (newTexture == nullptr) return;

	// Texture 최초 설정이 아닌 변경일 경우 Renderer Data 동기화..
	if (m_MaterialData->ORM != nullptr)
	{
		GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
	}

	// Texture 변경..
	m_MaterialData->ORM = newTexture;
	m_MaterialData->ORM->Name = ormName;
}

void Material::SetBaseColor(DirectX::SimpleMath::Vector4 color)
{
	m_MaterialData->Material_SubData->BaseColor = color;
}

void Material::SetAddColor(DirectX::SimpleMath::Vector4 color)
{
	m_MaterialData->Material_SubData->AddColor = color;
}

void Material::SetEmissiveFactor(float emissiveFactor)
{
	m_MaterialData->Material_SubData->EmissiveFactor = emissiveFactor;
}

void Material::SetRoughnessFactor(float roughnessFactor)
{
	m_MaterialData->Material_SubData->RoughnessFactor = roughnessFactor;
}

void Material::SetMetallicFactor(float metallicFactor)
{
	m_MaterialData->Material_SubData->MetallicFactor = metallicFactor;
}

void Material::Release()
{
	// Manager 내부에 있는 해당 Material Data 삭제..
	MaterialManager::DeleteMaterial(m_MaterialData->Material_Index);
	
	// 해당 Material Data 해제..
	delete m_MaterialData->Material_SubData;
	delete m_MaterialData;
}

std::string Material::GetDiffuseName()
{
	if (m_MaterialData->Albedo != nullptr )
	{
		return m_MaterialData->Albedo->Name;
	}

	return std::string();
}

std::string Material::GetNormalName()
{
	if (m_MaterialData->Normal != nullptr)
	{
		return m_MaterialData->Normal->Name;
	}

	return std::string();
}

std::string Material::GetEmissiveName()
{
	if (m_MaterialData->Emissive != nullptr)
	{
		return m_MaterialData->Emissive->Name;
	}

	return std::string();
}

std::string Material::GetORMName()
{
	if (m_MaterialData->ORM != nullptr)
	{
		return m_MaterialData->ORM->Name;
	}

	return std::string();
}
