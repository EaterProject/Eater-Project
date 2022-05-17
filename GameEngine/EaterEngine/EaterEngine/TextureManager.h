#pragma once

#include <string>
#include <windows.h>
#include <processenv.h>
#include <map>

class TextureBuffer;
class GraphicEngineManager;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
public:
	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void LoadTexture(std::string& Path);

public:
	void BakeSkyLightMap(std::string& Path, bool hdri);

	void BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri);
	void BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri);
	
public:
	void SaveConvertCubeMap(std::string& Path);

private:
	bool CheckFolder(std::string& Path);
	void LoadFolder(std::string& Path);
	void LoadFile(std::string& Path);
	std::string CutFilePath(std::string& Path);
private:
	std::map<std::string, TextureBuffer*> TextureList;
private:
	bool isFolder;

	static GraphicEngineManager*	m_Graphic;
	static CRITICAL_SECTION*		m_CriticalSection;
};

