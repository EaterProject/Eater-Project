#pragma once
#include <windows.h>
#include <string>

/// <summary>
///	여러개의 그래픽엔진을 쉽게 사용하기위해 만든 그래픽엔진 관리 매니저.
/// </summary>

class GraphicEngine;

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

class GraphicEngineManager
{
public:
	GraphicEngineManager();
	~GraphicEngineManager();

public:
	//선택한 그래픽엔진을 초기화
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, RenderOption* renderOption);

	//선택한 그래픽 엔진으로 Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//선택한 그래픽엔진 셋팅
	void RenderSetting();

	//선택한 그래픽엔진 랜더링
	void PushInstance(MeshData* meshData);
	void PushMesh(MeshBuffer* mesh);
	void PushMaterial(MaterialBuffer* material);
	void PushAnimation(AnimationBuffer* animation);

	void PushChangeInstance(MeshData* meshData);
	void PushChangeMesh(MeshBuffer* mesh);
	void PushChangeMaterial(MaterialBuffer* material);
	void PushChangeAnimation(AnimationBuffer* animation);

	void DeleteInstance(MeshData* meshData);
	void DeleteMesh(MeshBuffer* mesh);
	void DeleteMaterial(MaterialBuffer* material);
	void DeleteAnimation(AnimationBuffer* animation);

	void Render();
	void* PickingRender(int x, int y);

	void AddOccluder(MeshBuffer* occluder);

	//선택한 그래픽엔진으로 매쉬 버퍼 생성
	void CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource);

	//선택한 그래픽엔진으로 텍스쳐 생성
	void CreateTextureBuffer(std::string Name, TextureBuffer** ppResource);

	//선택한 그래픽엔진으로 애니메이션 버퍼 생성
	void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource);

public:
	//선택한 그래픽엔진으로 텍스쳐 생성
	void BakeSkyLightMap(TextureBuffer* skyLight, SkyLightBuffer** ppResource);
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, bool save_file, TextureBuffer** ppResource);

public:
	void SetEnvironment(TextureBuffer* environment);
	void SetSkyLight(SkyLightBuffer* skyLight);

private:
	GraphicEngine* GEngine;
};

