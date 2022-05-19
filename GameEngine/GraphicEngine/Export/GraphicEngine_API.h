#pragma once
#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicEngine_DLL.h"

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

class GraphicEngine_API
{
protected:
	GraphicEngine_API() {};
	virtual ~GraphicEngine_API() {};

public:
	/// Graphic Engine Create Function..
	static GRAPHIC_ENGINE_DLL GraphicEngine_API* Get();

public:
	/// Graphic Engine Funtion..
	virtual GRAPHIC_ENGINE_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight, RenderOption* renderOption) abstract;
	virtual GRAPHIC_ENGINE_DLL void OnReSize(int screenWidth, int screenHeight) abstract;
	virtual GRAPHIC_ENGINE_DLL void Release() abstract;

	/// Graphic Data Setting Function..
	virtual GRAPHIC_ENGINE_DLL void RenderSetting() abstract;
	virtual GRAPHIC_ENGINE_DLL void RenderSetting(RenderOption* renderOption) abstract;
	virtual GRAPHIC_ENGINE_DLL void SetGlobalData(GlobalData* globalData) abstract;

public:
	virtual GRAPHIC_ENGINE_DLL void SetSkyCube(TextureBuffer* resource) abstract;
	virtual GRAPHIC_ENGINE_DLL void SetSkyLight(SkyLightBuffer* resource) abstract;

public:
	/// Render Mesh Data & Rendering Function..
	virtual GRAPHIC_ENGINE_DLL void PushInstance(MeshData* instance) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_ENGINE_DLL void PushChangeInstance(MeshData* instance) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushChangeMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushChangeMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_ENGINE_DLL void PushChangeAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_ENGINE_DLL void DeleteInstance(MeshData* meshData) abstract;
	virtual GRAPHIC_ENGINE_DLL void DeleteMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_ENGINE_DLL void DeleteMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_ENGINE_DLL void DeleteAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_ENGINE_DLL void DeleteTexture(TextureBuffer* resource) abstract;
	virtual GRAPHIC_ENGINE_DLL void DeleteSkyLight(SkyLightBuffer* resource) abstract;

	virtual GRAPHIC_ENGINE_DLL void Render() abstract;
	virtual GRAPHIC_ENGINE_DLL void* PickingRender(int x, int y) abstract;

public:
	/// Graphic Resource Create Function..
	virtual GRAPHIC_ENGINE_DLL void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual GRAPHIC_ENGINE_DLL void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;
	virtual GRAPHIC_ENGINE_DLL void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) abstract;

public:
	/// Graphic Resource Baking Function..
	virtual GRAPHIC_ENGINE_DLL void BakeSkyLightMap(TextureBuffer* environment, bool hdri, SkyLightBuffer** ppResource) abstract;
	virtual GRAPHIC_ENGINE_DLL void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer** ppResource) abstract;

	virtual GRAPHIC_ENGINE_DLL void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName) abstract;

private:
	static GraphicEngine_API* Graphic;
};