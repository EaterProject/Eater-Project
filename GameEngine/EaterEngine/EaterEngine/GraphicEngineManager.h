#pragma once
#include <windows.h>
#include <map>
#include <string>
#include <queue>
#include "BaseManager.h"

/// <summary>
///	�������� �׷��ȿ����� ���� ����ϱ����� ���� �׷��ȿ��� ���� �Ŵ���.
/// </summary>

class ObjectManager;
class MeshData;
class GlobalData;
class GraphicEngine;
class LoadMeshData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;

namespace ParserData
{
	class Mesh;
}

class GraphicEngineManager : public BaseManager
{
public:
	GraphicEngineManager();
	~GraphicEngineManager();
public:
	//������ �׷��ȿ����� �ʱ�ȭ
	void Initialize(HWND Hwnd, UINT& renderOption, int WinSizeWidth, int WinSizeHeight,ObjectManager* GM);

	//������ �׷��� �������� Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//������ �׷��ȿ��� ����
	void RenderSetting(UINT& renderOption);
	void SetEnvironment(bool enable);

	//������ �׷��ȿ��� ������
	void AddMeshData(MeshData* mesh);
	void DeleteMeshData(MeshData* mesh);
	void Render();

	void LoadEnvironment(std::string mPath);

	void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector4 color);
	void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector4 color);

	//������ �׷��ȿ������� �ε������۸� ������
	void CreateMeshBuffer(ParserData::Mesh* mModel, LoadMeshData* meshData);

	//������ �׷��ȿ������� �ؽ��� ����
	TextureBuffer* CreateTextureBuffer(std::string Name);

private:
	GraphicEngine* GEngine;
	ObjectManager* ObjManager;
};

