#pragma once
#include <string>
#include "Component.h"
#include "EaterEngineDLL.h"

class MeshData;
class MaterialData;
class LoadMeshData;
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

	void PushMaterialData(LoadMeshData* mesh);

	void SetTexTransform(DirectX::SimpleMath::Vector3 scale);
	void SetTexTransform(float x, float y, float z);

	void SetDiffuseMap(std::string diffuseName);
	void SetNormalMap(std::string noramlName);

	void SetBaseColor(DirectX::SimpleMath::Vector4 color);
	void SetStartColor(DirectX::SimpleMath::Vector4 color);

public:
	void Release();

public:
	UINT GetMaterialIndex();
	MaterialData* GetMaterialData();

private:
	MeshData* m_MeshData;

	MaterialData* MaterialDatas;
};

