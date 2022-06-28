#pragma once
#include <windows.h>
#include <string>

/// <summary>
///	�������� �׷��ȿ����� ���� ����ϱ����� ���� �׷��ȿ��� ���� �Ŵ���.
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
	//������ �׷��ȿ����� �ʱ�ȭ
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, RenderOption* renderOption);

	//������ �׷��� �������� Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//������ �׷��ȿ��� ����
	void RenderSetting();

	//������ �׷��ȿ��� ������
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

	//������ �׷��ȿ������� �Ž� ���� ����
	void CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource);

	//������ �׷��ȿ������� �ؽ��� ����
	void CreateTextureBuffer(std::string Name, TextureBuffer** ppResource);

	//������ �׷��ȿ������� �ִϸ��̼� ���� ����
	void CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource);

public:
	//������ �׷��ȿ������� �ؽ��� ����
	void BakeSkyLightMap(TextureBuffer* skyLight, bool hdri, SkyLightBuffer** ppResource);
	void BakeConvertCubeMap(TextureBuffer* resource, float angle, float threshold, bool hdri, TextureBuffer** ppResource);

	void SaveConvertCubeMap(TextureBuffer* resource, std::string SaveName);
	void SaveSpriteToVolumeTexture(TextureBuffer* resource, std::string saveName, UINT pixelSize, TextureBuffer** ppResource);
	void SaveSpriteToVolumeTexture(std::string fileName, std::string saveName, UINT pixelSize);

public:
	void SetSkyCube(TextureBuffer* environment);
	void SetSkyLight(SkyLightBuffer* skyLight, UINT index);
	void SetColorGradingBaseTexture(TextureBuffer* lut_resource);
	void SetColorGradingBlendTexture(TextureBuffer* lut_resource);
	void SetColorGradingBlendFactor(float blend_factor);

public:
	void SetFullScreenBlur(bool enable, UINT blur_count);

private:
	GraphicEngine* GEngine;
};

