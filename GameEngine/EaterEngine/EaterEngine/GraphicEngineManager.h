#pragma once
#include <windows.h>
#include <string>

/// <summary>
///	여러개의 그래픽엔진을 쉽게 사용하기위해 만든 그래픽엔진 관리 매니저.
/// </summary>

class GraphicEngine;
class MeshData;
class MeshBuffer;
class MaterialBuffer;
class TextureBuffer;
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
	//선택한 그래픽엔진을 초기화
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight);

	//선택한 그래픽 엔진으로 Resize
	void OnReSize(int Change_Width, int Change_Height);
	void Release();

	//선택한 그래픽엔진 셋팅
	void RenderSetting(RenderOption* renderOption);


	//선택한 그래픽엔진 랜더링
	void PushInstance(MeshData* mesh);
	void PushMesh(MeshBuffer* meshData);
	void PushMaterial(MaterialBuffer* materialData);
	void PushChangeInstance(MeshData* mesh);
	void DeleteInstance(MeshData* mesh);

	void Render();
	void* PickingRender(int x, int y);

	void AddOccluder(MeshBuffer* occluder);

	//선택한 그래픽엔진으로 인덱스버퍼를 생성함
	void CreateMeshBuffer(ParserData::CMesh* model, MeshBuffer** ppResource);

	//선택한 그래픽엔진으로 텍스쳐 생성
	void CreateTextureBuffer(std::string Name, TextureBuffer** ppResource);

public:
	//선택한 그래픽엔진으로 텍스쳐 생성
	void BakeShadowMap(std::string Path);
	void BakeEnvironmentMap(TextureBuffer* environment, EnvironmentBuffer** ppResource);

public:
	void SetShadowMap(TextureBuffer* resource);
	void SetEnvironmentMap(EnvironmentBuffer* resource);


private:
	GraphicEngine* GEngine;
};

