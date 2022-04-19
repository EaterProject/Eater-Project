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
class EnvironmentBuffer;
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
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight);

	//������ �׷��� �������� Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//������ �׷��ȿ��� ����
	void RenderSetting(RenderOption* renderOption);


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
	void BakeShadowMap(std::string Path);
	void BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource);

public:
	void SetShadowMap(TextureBuffer* resource);
	void SetEnvironmentMap(EnvironmentBuffer* resource);

private:
	GraphicEngine* GEngine;
};

