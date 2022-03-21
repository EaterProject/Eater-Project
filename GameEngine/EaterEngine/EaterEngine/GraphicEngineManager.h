#pragma once
#include <windows.h>
#include <map>
#include <string>
#include <queue>
#include "BaseManager.h"

/// <summary>
///	여러개의 그래픽엔진을 쉽게 사용하기위해 만든 그래픽엔진 관리 매니저.
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
	//선택한 그래픽엔진을 초기화
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight);

	//선택한 그래픽 엔진으로 Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//선택한 그래픽엔진 셋팅
	void RenderSetting(RenderOption* renderOption);
	void SetEnvironment(bool enable);

	//선택한 그래픽엔진 랜더링
	void PushInstance(MeshData* mesh);
	void PushMesh(MeshBuffer* meshData);
	void PushMaterial(MaterialBuffer* materialData);
	void PushChangeInstance(MeshData* mesh);
	void DeleteInstance(MeshData* mesh);
	void Render();

	void LoadEnvironment(std::string mPath);

	void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Vector4 color);
	void DebugDrawLine(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 dir, float distance, DirectX::SimpleMath::Vector4 color);

	//선택한 그래픽엔진으로 인덱스버퍼를 생성함
	void CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource);

	//선택한 그래픽엔진으로 텍스쳐 생성
	void CreateTextureBuffer(std::string Name, TextureBuffer** ppResource);

private:
	GraphicEngine* GEngine;
};

