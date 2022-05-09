#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicDLL.h"

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
class EnvironmentBuffer;
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
	static GRAPHIC_DLL GraphicEngine* Get();

public:
	/// Graphic Engine Funtion..
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight, RenderOption* renderOption) abstract;
	virtual GRAPHIC_DLL void OnReSize(int screenWidth, int screenHeight) abstract;				
	virtual GRAPHIC_DLL void Release() abstract;

	/// Graphic Data Setting Function..
	virtual GRAPHIC_DLL void RenderSetting() abstract;
	virtual GRAPHIC_DLL void RenderSetting(RenderOption* renderOption) abstract;
	virtual GRAPHIC_DLL void SetGlobalData(GlobalData* globalData) abstract;

public:
	virtual GRAPHIC_DLL void SetEnvironmentMap(EnvironmentBuffer* resource) abstract;

public:
	/// Render Mesh Data & Rendering Function..
	virtual GRAPHIC_DLL void PushInstance(MeshData* instance) abstract;
	virtual GRAPHIC_DLL void PushMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DLL void PushMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DLL void PushAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DLL void PushChangeInstance(MeshData* instance) abstract;
	virtual GRAPHIC_DLL void PushChangeMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DLL void PushChangeMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DLL void PushChangeAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DLL void DeleteInstance(MeshData* meshData) abstract;
	virtual GRAPHIC_DLL void DeleteMesh(MeshBuffer* mesh) abstract;
	virtual GRAPHIC_DLL void DeleteMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DLL void DeleteAnimation(AnimationBuffer* animation) abstract;

	virtual GRAPHIC_DLL void Render() abstract;
	virtual GRAPHIC_DLL void* PickingRender(int x, int y) abstract;

public:
	/// Graphic Resource Create Function..
	virtual GRAPHIC_DLL void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;
	virtual GRAPHIC_DLL void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource) abstract;

public:
	/// Graphic Resource Baking Function..
	virtual GRAPHIC_DLL void BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource) abstract;

private:
	static GraphicEngine* Graphic;
};
