#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicDLL.h"

/// <summary>
/// ���� �������� �����ϴ� �׷��� ���� �ֻ��� Ŭ����
/// �ش� Ŭ������ ���� Graphic Engine�� ���� �� �ִ�..
/// </summary>

class MeshData;
class MeshBuffer;
class TextureBuffer;
class MaterialBuffer;
class GlobalData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;
class LoadMeshData;
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
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) abstract;
	virtual GRAPHIC_DLL void OnReSize(int screenWidth, int screenHeight) abstract;				
	virtual GRAPHIC_DLL void Release() abstract;

	/// Graphic Data Setting Function..
	virtual GRAPHIC_DLL void RenderSetting(RenderOption* renderOption) abstract;
	virtual GRAPHIC_DLL void SetGlobalData(GlobalData* globalData) abstract;
	virtual GRAPHIC_DLL void SetEnvironmentMap(bool enable) abstract;

	/// Render Mesh Data & Rendering Function..
	virtual GRAPHIC_DLL void PushInstance(MeshData* meshData) abstract;
	virtual GRAPHIC_DLL void PushMaterial(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DLL void PushMesh(MeshBuffer* mesh) abstract;

	virtual GRAPHIC_DLL void AddChangeMeshData(MeshData* meshData) abstract;
	virtual GRAPHIC_DLL void AddChangeMaterialData(MaterialBuffer* material) abstract;
	virtual GRAPHIC_DLL void DeleteMeshData(MeshData* meshData) abstract;

	virtual GRAPHIC_DLL void Render() abstract;

	/// Graphic Resource Create Function..
	virtual GRAPHIC_DLL void CreateTextureBuffer(std::string path, TextureBuffer** ppResource) abstract;
	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource) abstract;

	virtual GRAPHIC_DLL void CreateEnvironmentMap(std::string path) abstract;

private:
	static GraphicEngine* Graphic;
};
