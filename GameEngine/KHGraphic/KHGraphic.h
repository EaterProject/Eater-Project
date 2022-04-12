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

public:
	void SetShadowMap(TextureBuffer* resource) override;
	void SetEnvironmentMap(EnvironmentBuffer* resource) override;

public:
	void PushInstance(MeshData* instance) override;
	void PushMesh(MeshBuffer* mesh) override;
	void PushMaterial(MaterialBuffer* material) override;

	void PushChangeInstance(MeshData* instance) override;
	void PushChangeMesh(MeshBuffer* mesh) override;
	void PushChangeMaterial(MaterialBuffer* material) override;

	void DeleteInstance(MeshData* meshData) override;
	void DeleteMesh(MeshBuffer* mesh) override;
	void DeleteMaterial(MaterialBuffer* material) override;

	void Render() override;
	void* PickingRender(int x, int y) override;

public:
	void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) override;
	void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) override;

public:
	void BakeShadowMap(std::string path) override;
	void BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource) override;

private:
	IFactoryManager* m_FactoryManager;
	IRenderManager* m_RenderManager;
};

