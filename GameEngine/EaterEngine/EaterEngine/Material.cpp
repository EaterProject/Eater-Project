#include "Material.h"
#include "EngineData.h"
#include "MaterialManager.h"
#include "LoadManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Material::Material()
	:Defalt(false)
{
	// Material Buffer 생성..
	m_MaterialData = new MaterialBuffer();

	// Material Data 생성..
	m_MaterialData->Material_SubData = new MaterialSubData();

	m_MaterialData->Material_SubData->Tile = { 1.0f, 1.0f };
	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// Material 등록..
	MaterialManager::PushMaterial(this);

	// Material Graphic 측 등록..
	GraphicEngine::Get()->PushMaterial(m_MaterialData);
}

Material::~Material()
{
	// Manager 내부에 있는 해당 Material Data 삭제..
	MaterialManager::DeleteMaterial(m_MaterialData->BufferIndex);
}

void Material::SetTextureTiling(float scale_x, float scale_y)
{
	// 설정 Tiling에 따른 Texture Transform 설정..
	m_MaterialData->Material_SubData->Tile = { scale_x, scale_y };
	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f / scale_x, 1.0f / scale_y, 1.0f);
}

void Material::SetDiffuseTexture(std::string diffuseName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(diffuseName);

	// Texture 변경..
	m_MaterialData->Albedo = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Albedo->Name = diffuseName;
	}

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);
	
	// Texture 변경..
	m_MaterialData->Normal = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Normal->Name = noramlName;
	}

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// Texture 변경..
	m_MaterialData->Emissive = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Emissive->Name = emissiveName;
	}

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// Texture 변경..
	m_MaterialData->ORM = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->ORM->Name = ormName;
	}

	// Renderer Data 동기화..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
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
	// Graphic 내부에 있는 해당 Materail Buffer 삭제..
	GraphicEngine::Get()->DeleteMaterial(m_MaterialData);

	// 해당 Material Data 해제..
	delete m_MaterialData;
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
