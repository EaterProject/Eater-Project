#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>
#include <string>

class Collider;
class MeshFilter;
class TerrainData;
class Terrain : public Component
{
public:
	EATER_ENGINEDLL Terrain();
	virtual ~Terrain();

public:
	void Awake() override;
	void SetUp() override;
	void Start() override;

public:
	EATER_ENGINEDLL void SetLayerName(std::string diffuseName, std::string normalName, std::string ormName);
	EATER_ENGINEDLL void SetColliderName(std::string TerrainColliderName);
	EATER_ENGINEDLL void SetMeshName(std::string TerrainMeshName);

	EATER_ENGINEDLL void SetTextureTiling(float scale);
	EATER_ENGINEDLL void SetTextureTiling(float scale_x, float scale_y);

public:
	void Release();

private:
	void AddLayer(std::string diffuseName, std::string normalName, std::string ormName);
	void DataUpdate();

private:
	TerrainData* m_TerrainData;
	std::vector<Material*> m_MaterialLayer;
	
	bool isLoad_Data = false;
	UINT LayerSize = 0;
	DirectX::SimpleMath::Vector2 Tiling;

	std::vector<std::string> DiffuseNameList;
	std::vector<std::string> NormalNameList;
	std::vector<std::string> ORMNameList;

	std::string TerrainColliderName;
	std::string TerrainMeshName;

	Collider*	mCollider;
	MeshFilter* mMeshFilter;
};