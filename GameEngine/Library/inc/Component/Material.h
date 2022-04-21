#pragma once
#include <string>
#include "EaterEngineDLL.h"

class MaterialBuffer;
class MaterialManager;

class Material
{
public:
	EATER_ENGINEDLL Material();
	~Material();

public:
	friend class MaterialManager;

public:
	EATER_ENGINEDLL void SetTextureTiling(float scale_x, float scale_y);

	EATER_ENGINEDLL void SetDiffuseTexture(std::string diffuseName);
	EATER_ENGINEDLL void SetNormalTexture(std::string noramlName);
	EATER_ENGINEDLL void SetEmissiveTexture(std::string emissiveName);
	EATER_ENGINEDLL void SetORMTexture(std::string ormName);

	EATER_ENGINEDLL void SetAddColor(DirectX::SimpleMath::Vector3 color);

	EATER_ENGINEDLL void SetEmissiveFactor(float emissiveFactor);
	EATER_ENGINEDLL void SetRoughnessFactor(float roughnessFactor);
	EATER_ENGINEDLL void SetMetallicFactor(float metallicFactor);

public:
	void Release();

public:
	EATER_ENGINEDLL std::string GetDiffuseName();
	EATER_ENGINEDLL std::string GetNormalName();
	EATER_ENGINEDLL std::string GetEmissiveName();
	EATER_ENGINEDLL std::string GetORMName();

public:
	bool Defalt;
	std::string Name;

	MaterialBuffer* m_MaterialData;
};

