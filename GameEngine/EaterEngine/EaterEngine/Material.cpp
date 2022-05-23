#include "Material.h"
#include "EngineData.h"
#include "LoadManager.h"
#include "GraphicEngineAPI.h"
#include "IndexManager.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Material::Material()
	:Defalt(false)
{
	// Material Buffer ����..
	m_MaterialData = new MaterialBuffer();

	// Material Data ����..
	m_MaterialData->Material_Property = new MaterialProperty();

	m_MaterialData->Material_Property->Tile = { 1.0f, 1.0f };
	m_MaterialData->Material_Property->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// Material ���..
	IndexManager<MaterialBuffer>::PushResource(m_MaterialData, &m_MaterialData->BufferIndex);

	// Material Graphic �� ���..
	GraphicEngine::Get()->PushMaterial(m_MaterialData);
}

Material::~Material()
{
	// �ش� Resource ����..
	Release();

	// Manager ���ο� �ִ� �ش� Material Data ����..
	IndexManager<MaterialBuffer>::DeleteResource(m_MaterialData->BufferIndex);
}

void Material::SetTextureTiling(float scale_x, float scale_y)
{
	// ���� Tiling�� ���� Texture Transform ����..
	m_MaterialData->Material_Property->Tile = { scale_x, scale_y };
	m_MaterialData->Material_Property->TexTM = DirectX::SimpleMath::Matrix::CreateScale(1.0f / scale_x, 1.0f / scale_y, 1.0f);
}

void Material::SetDiffuseTexture(std::string diffuseName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(diffuseName);

	// Texture ����..
	m_MaterialData->Albedo = newTexture;

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);
	
	// Texture ����..
	m_MaterialData->Normal = newTexture;

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// Texture ����..
	m_MaterialData->Emissive = newTexture;

	// Renderer Data ����ȭ..
	GraphicEngine::Get()->PushChangeMaterial(m_MaterialData);
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// Texture ����..
	m_MaterialData->ORM = newTexture;

	// Renderer Data ����ȭ..
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

void Material::Release()
{
	// Graphic ���ο� �ִ� �ش� Materail Buffer ����..
	GraphicEngine::Get()->DeleteMaterial(m_MaterialData);

	// �ش� Material Data ����..
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
