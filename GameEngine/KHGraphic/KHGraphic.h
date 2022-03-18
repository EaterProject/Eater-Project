#pragma once
#include "GraphicsEngine.h"

interface IRenderManager;
interface IFactoryManager;

class KHGraphic : public GraphicEngine
{
public:
	KHGraphic();
	~KHGraphic();

public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;
	void OnReSize(int screenWidth, int screenHeight) override;
	void Release() override;

public:
	void RenderSetting(RenderOption* renderOption) override;
	void SetGlobalData(GlobalData* globalData) override;
	void SetEnvironmentMap(bool enable) override;

public:
	void PushInstance(MeshData* meshData) override;
	void PushMaterial(MaterialBuffer* material) override;
	void PushMesh(MeshBuffer* mesh) override;

	void AddChangeMeshData(MeshData* meshData) override;
	void AddChangeMaterialData(MaterialBuffer* materialData) override;
	void DeleteMeshData(MeshData* meshData) override;
	void Render() override;

public:
	void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) override;
	void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) override;
	void CreateEnvironmentMap(std::string path) override;

private:
	IFactoryManager* m_FactoryManager;
	IRenderManager* m_RenderManager;
};

