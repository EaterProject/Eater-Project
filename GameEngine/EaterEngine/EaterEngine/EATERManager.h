#pragma once
#include <windows.h>
#include <string>
#include <vector>
class GraphicEngineManager;
class LoadMeshData;
class ModelData;
class GameObject;

class Eater_LoadBuffer;
class Eater_LoadMaterial;
class Eater_LoadMesh;
class Eater_LoadScene;

namespace ParserData
{
	class CMesh;
	class CAnimation;
}
class Mesh;
class EATERManager
{
public:
	EATERManager();
	~EATERManager();

	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void Load(std::string& Path, UINT parsingMode);
	void LoadScene(std::string& Path);
	void LoadMesh(std::string& Path);
	void LoadMaterial(std::string& Path);
	void LoadPrefap(std::string& path);
private:
	GraphicEngineManager* m_Graphic;
	CRITICAL_SECTION* m_CriticalSection;

	//각각의 파일을 로드하는 매니저들
	Eater_LoadBuffer*		mEaterBuffer;
	Eater_LoadMaterial*		mEaterMaterial;
	Eater_LoadMesh*			mEaterMesh;
	Eater_LoadScene*		mEaterScene;
};

