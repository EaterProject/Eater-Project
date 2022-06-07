#include "Material.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "GraphicEngineAPI.h"
#include "IndexManager.h"
#include "MaterialManager.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Material::Material()
	:is_Default(false)
{
	// Material Buffer 생성..
	m_MaterialData = new MaterialBuffer();

	// Material Data 생성..
	m_MaterialData->Material_Property = new MaterialProperty();

	m_MaterialData->Material_Property->Tile = { 1.0f, 1.0f };
	m_MaterialData->Material_Property->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// Material 등록..
	MaterialManager::SetIndex(&m_MaterialData->BufferIndex);

	// Material Graphic 측 등록..
	GraphicEngine::Get()->PushMaterial(m_MaterialData);
}

Material::~Material()
{
	// Manager 내부에 있는 해당 Material Data 삭제..
	MaterialManager::DeleteIndex(m_MaterialData->BufferIndex);

	// 해당 Resource 제거..
	Release();
}

void Material::SetTextureTiling(float scale_x, float scale_y)
{
	// 설정 Tiling에 따른 Texture Transform 설정..
	m_MaterialData->Material_Property->Tile = { scale_x, scale_y };
	m_MaterialData->Material_Property->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f / scale_x, 1.0f / scale_y, 1.0f);
}

void Material::SetDiffuseTexture(std::string diffuseName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(diffuseName);

	// Texture 변경..
	m_MaterialData->Albedo = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);
	
	// Texture 변경..
	m_MaterialData->Normal = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// Texture 변경..
	m_MaterialData->Emissive = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// Texture 변경..
	m_MaterialData->ORM = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetAddColor(DirectX::SimpleMath::Vector3 color)
{
	m_MaterialData->Material_Property->AddColor = color;
}

void Material::SetEmissiveColor(DirectX::SimpleMath::Vector3 color)
{
	m_MaterialData->Material_Property->EmissiveColor = color;
}

void Material::SetEmissiveFactor(float emissiveFactor)
{
	m_MaterialData->Material_Property->EmissiveFactor = emissiveFactor;
}

void Material::SetRoughnessFactor(float roughnessFactor)
{
	m_MaterialData->Material_Property->RoughnessFactor = roughnessFactor;
}

void Material::SetMetallicFactor(float metallicFactor)
{
	m_MaterialData->Material_Property->MetallicFactor = metallicFactor;
}

void Material::SetSkyLightIndex(int skyLightIndex)
{
	m_MaterialData->Material_Property->SkyLightIndex = skyLightIndex;
}

void Material::Release()
{
	// Graphic 내부에 있는 해당 Materail Buffer 삭제..
	GraphicEngine::Get()->DeleteMaterial(m_MaterialData);

	// 해당 Material Data 해제..
	SAFE_DELETE(m_MaterialData);
}

std::string Material::GetDiffuseName()
{
	if (m_MaterialData->Albedo != nullptr)
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

bool Material::operator<(const Material& material) const
{
	if (m_MaterialData->BufferIndex != material.m_MaterialData->BufferIndex)
	{
		return m_MaterialData->BufferIndex < material.m_MaterialData->BufferIndex;
	}

	return false;
}
