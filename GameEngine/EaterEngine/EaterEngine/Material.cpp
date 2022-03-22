#include "Material.h"
#include "EngineData.h"
#include "MaterialManager.h"
#include "LoadManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Material::Material()
	:Defalt(false)
{
	// Material Buffer ����..
	m_MaterialData = new MaterialBuffer();

	// Material Data ����..
	m_MaterialData->Material_SubData = new MaterialSubData();

	m_MaterialData->Material_SubData->Tile = { 1.0f, 1.0f };
	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// Material ���..
	MaterialManager::PushMaterial(this);

	// Material Graphic �� ���..
	GraphicEngine::Get()->PushMaterial(m_MaterialData);
}

Material::~Material()
{
	// Manager ���ο� �ִ� �ش� Material Data ����..
	MaterialManager::DeleteMaterial(m_MaterialData->BufferIndex);
}

void Material::SetTextureTiling(float scale_x, float scale_y)
{
	// ���� Tiling�� ���� Texture Transform ����..
	m_MaterialData->Material_SubData->Tile = { scale_x, scale_y };
	m_MaterialData->Material_SubData->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f / scale_x, 1.0f / scale_y, 1.0f);
}

void Material::SetDiffuseTexture(std::string diffuseName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(diffuseName);

	// Texture ����..
	m_MaterialData->Albedo = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Albedo->Name = diffuseName;
	}

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);
	
	// Texture ����..
	m_MaterialData->Normal = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Normal->Name = noramlName;
	}

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// Texture ����..
	m_MaterialData->Emissive = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->Emissive->Name = emissiveName;
	}

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// Texture ����..
	m_MaterialData->ORM = newTexture;

	if (newTexture != nullptr)
	{
		m_MaterialData->ORM->Name = ormName;
	}

	// Renderer Data ����ȭ..
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
	// Graphic ���ο� �ִ� �ش� Materail Buffer ����..
	GraphicEngine::Get()->DeleteMaterial(m_MaterialData);

	// �ش� Material Data ����..
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
