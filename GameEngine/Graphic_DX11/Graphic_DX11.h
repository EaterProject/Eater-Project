#pragma once
#include "GraphicEngineAPI.h"

interface IRenderManager;
interface IFactoryManager;
interface IRenderDataConverter;

class Graphic_DX11 : public GraphicEngine
{
public:
	Graphic_DX11();
	~Graphic_DX11();

public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight, RenderOption* renderOption) override;
	void OnReSize(int screenWidth, int screenHeight) override;
	void Release() override;

public:
	void RenderSetting() override;
	void RenderSetting(RenderOption* renderOption) override;

	void SetGlobalData(GlobalData* globalData) override;

public:
	void SetSkyCube(TextureBuffer* resource) override;
	void SetSkyLight(SkyLightBuffer* resource, UINT index) override;
	void SetColorGradingBaseTexture(TextureBuffer* lut_resource) override;
	void SetColorGradingBlendTexture(TextureBuffer* lut_resource) override;

public:
	void PushInstance(MeshData* instance) override;
	void PushMesh(MeshBuffer* mesh) override;
	void PushMaterial(MaterialBuffer* material) override;
	void PushAnimation(AnimationBuffer* animation) override;

	void PushChangeInstance(MeshData* instance) override;
	void PushChangeMesh(MeshBuffer* mesh) override;
	void PushChangeMaterial(MaterialBuffer* material) override;
	void PushChangeAnimation(AnimationBuffer* animation) override;

	void DeleteInstance(MeshData* meshData) override;
	void DeleteMesh(MeshBuffer* mesh) override;
	void DeleteMaterial(MaterialBuffer* material) override;
	void DeleteAnimation(AnimationBuffer* animation) override;

	void PushMaterialBlockInstance(MeshData* instance);
	void PopMaterialBlockInstance(MeshData* instance);

	void DeleteTexture(TextureBuffer* resource) override;
	void DeleteSkyLight(SkyLightBuffer* resource) override;

	void Render() override;
	void* PickingRender(int x, int y) override;

public:
	void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) override;
	void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) override;
	void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) override;

public:
	void BakeSkyLightMap(TextureBuffer* environment, bool hdri, SkyLightBuffer** ppResource) override;
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer** ppResource) override;

	void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName) override;
	void SaveSpriteToVolumeTexture(TextureBuffer* resource, std::string saveName, UINT pixelSize, TextureBuffer** ppResource) override;
	void SaveSpriteToVolumeTexture(std::string fileName, std::string saveName, UINT pixelSize) override;

private:
	IFactoryManager* m_FactoryManager;
	IRenderManager* m_RenderManager;
	IRenderDataConverter* m_Converter;
};

