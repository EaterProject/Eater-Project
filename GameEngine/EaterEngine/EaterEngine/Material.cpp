#include "Material.h"
#include "EngineData.h"
#include "MaterialManager.h"
#include "LoadManager.h"
#include "GraphicsEngine.h"

#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }

Material::Material()
{
	// Material Buffer 생성..
	m_MaterialData = new MaterialBuffer();

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

	// Texture 변경..
	m_MaterialData->Albedo = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
}

void Material::SetNormalTexture(std::string noramlName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(noramlName);

	// Texture 변경..
	m_MaterialData->Normal = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
}

void Material::SetEmissiveTexture(std::string emissiveName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(emissiveName);

	// Texture 변경..
	m_MaterialData->Emissive = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
}

void Material::SetORMTexture(std::string ormName)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(ormName);

	// Texture 변경..
	m_MaterialData->ORM = newTexture;

	// Renderer Data 동기화..
	GraphicEngine::Get()->AddChangeMaterialData(m_MaterialData);
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
	MaterialManager::DeleteMaterial(m_MaterialData->BufferIndex);
	
	// 해당 Material Data 해제..
	SAFE_DELETE(m_MaterialData->Material_SubData);
	SAFE_DELETE(m_MaterialData);
}