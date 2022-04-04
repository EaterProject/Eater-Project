#pragma once
#include <windows.h>
#include <string>
#include "BaseManager.h"

/// <summary>
///	�������� �׷��ȿ����� ���� ����ϱ����� ���� �׷��ȿ��� ���� �Ŵ���.
/// </summary>

class MeshData;
class GlobalData;
class GraphicEngine;
class LoadMeshData;
class MeshBuffer;
class MaterialBuffer;
class TextureBuffer;
class RenderOption;

namespace ParserData
{
	class CMesh;
}

class GraphicEngineManager : public BaseManager
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
	void SetEnvironment(bool enable);

	//������ �׷��ȿ��� ������
	void PushInstance(MeshData* mesh);
	void PushMesh(MeshBuffer* meshData);
	void PushMaterial(MaterialBuffer* materialData);
	void PushChangeInstance(MeshData* mesh);
	void DeleteInstance(MeshData* mesh);

	void Render();
	void* PickingRender(int x, int y);

	void LoadEnvironment(std::string mPath);

	//������ �׷��ȿ������� �ε������۸� ������
	void CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource);

	//������ �׷��ȿ������� �ؽ��� ����
	void CreateTextureBuffer(std::string Name, TextureBuffer** ppResource);

private:
	GraphicEngine* GEngine;
};

