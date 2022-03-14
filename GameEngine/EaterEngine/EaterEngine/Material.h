#pragma once
#include <string>
#include "Component.h"
#include "EaterEngineDLL.h"

class MeshData;
class MaterialData;
class TextureBuffer;
class MaterialManager;

class Material
{
public:
	Material();
	~Material();
public:
	friend class MaterialManager;

public:
	void SetMeshData(MeshData* meshData);
	void SetMaterialIndex(UINT index);

	void SetTexTransform(DirectX::SimpleMath::Vector3 scale);
	void SetTexTransform(float x, float y, float z);

	void SetDiffuseMap(std::string diffuseName);
	void SetNormalMap(std::string noramlName);

	void SetBaseColor(DirectX::SimpleMath::Vector4 color);
	void SetStartColor(DirectX::SimpleMath::Vector4 color);

	void SetEmissiveFactor(float emissiveFactor);
	void SetRoughnessFactor(float roughnessFactor);
	void SetMetallicFactor(float metallicFactor);

public:
	void Release();

public:
	UINT GetMaterialIndex();
	MaterialData* GetMaterialData();

private:
	MeshData* m_MeshData;

	MaterialData* MaterialDatas;
};

