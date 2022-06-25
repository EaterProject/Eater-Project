#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "Graphic_DX11_DLL.h"

/// <summary>
/// 게임 엔진에서 제공하는 그래픽 엔진 최상위 클래스
/// 해당 클래스를 통해 Graphic Engine을 받을 수 있다..
/// </summary>

class GlobalData;
class MeshData;

class ModelData;
class ModelAnimationData;

class MeshBuffer;
class MaterialBuffer;
class TextureBuffer;
class AnimationBuffer;
class SkyLightBuffer;
class RenderOption;

namespace ParserData 
{
	class CMesh;
}

class GraphicEngine
{
protected:
	GraphicEngine() {};
	virtual ~GraphicEngine() {};

public:
	/// Graphic Engine Create Function..
	static GRAPHIC_DX11_DLL GraphicEngine* Get();

public:
	/// Graphic Engine Funtion..
	virtual GRAPHIC_DX11_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight, RenderOption* renderOption) abstract;
	virtual GRAPHIC_DX11_DLL void OnReSize(int screenWidth, int screenHeight) abstract;				
	virtual GRAPHIC_DX11_DLL void Release() abstract;

	/// Graphic Data Setting Function..
	virtual GRAPHIC_DX11_DLL void RenderSetting() abstract;
	virtual GRAPHIC_DX11_DLL void RenderSetting(RenderOption* renderOption) abstract;
	virtual GRAPHIC_DX11_DLL void SetGlobalData(GlobalData* globalData) abstract;

public:
	virtual GRAPHIC_DX11_DLL void SetSkyCube(TextureBuffer* resource) abstract;
	virtual GRAPHIC_DX11_DLL void SetSkyLight(SkyLightBuffer* resource, UINT index) abstract;
	virtual GRAPHIC_DX11_DLL void SetColorGradingBaseTexture(TextureBuffer* lut_resource) abstract;
	virtual GRAPHIC_DX11_DLL void SetColorGradingBlendTexture(TextureBuffer* lut_resource) abstract;
	//virtual GRAPHIC_DX11_DLL void SetColorGradingFactor(float factor) abstract;

public:
	/// Render Mesh Data & Rendering Function..
	virtual GRAPHIC_DX11_DLL void PushInstance(MeshData* instance) abstract;
	virtual GRAPHIC_DX11_DLL void PushMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DX11_DLL void PushMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DX11_DLL void PushAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DX11_DLL void PushChangeInstance(MeshData* instance) abstract;
	virtual GRAPHIC_DX11_DLL void PushChangeMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DX11_DLL void PushChangeMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DX11_DLL void PushChangeAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DX11_DLL void DeleteInstance(MeshData* meshData) abstract;
	virtual GRAPHIC_DX11_DLL void DeleteMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DX11_DLL void DeleteMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DX11_DLL void DeleteAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DX11_DLL void PushMaterialBlockInstance(MeshData* instance) abstract;
	virtual GRAPHIC_DX11_DLL void PopMaterialBlockInstance(MeshData* instance) abstract;

	virtual GRAPHIC_DX11_DLL void DeleteTexture(TextureBuffer* resource) abstract;
	virtual GRAPHIC_DX11_DLL void DeleteSkyLight(SkyLightBuffer* resource) abstract;

	virtual GRAPHIC_DX11_DLL void Render() abstract;
	virtual GRAPHIC_DX11_DLL void* PickingRender(int x, int y) abstract;

public:
	/// Graphic Resource Create Function..
	virtual GRAPHIC_DX11_DLL void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual GRAPHIC_DX11_DLL void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;
	virtual GRAPHIC_DX11_DLL void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) abstract;

public:
	/// Graphic Resource Baking Function..
	virtual GRAPHIC_DX11_DLL void BakeSkyLightMap(TextureBuffer* environment, bool hdri, SkyLightBuffer** ppResource) abstract;
	virtual GRAPHIC_DX11_DLL void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer** ppResource) abstract;

	virtual GRAPHIC_DX11_DLL void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName) abstract;
	virtual GRAPHIC_DX11_DLL void SaveSpriteToVolumeTexture(TextureBuffer* resource, std::string saveName, UINT pixelSize, TextureBuffer** ppResource) abstract;
	virtual GRAPHIC_DX11_DLL void SaveSpriteToVolumeTexture(std::string fileName, std::string saveName, UINT pixelSize) abstract;

private:
	static GraphicEngine* Graphic;
};
