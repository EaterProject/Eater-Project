#pragma once

/// <summary>
/// 매쉬,텍스쳐,프리팹,등등 그리기위해서 필요한것들을 로드
/// </summary>



#include <string>
#include <map>
#include <vector>
#include "ParserData.h"
#include "SimpleMath.h"

namespace ParserData
{
	struct Model;
	class Mesh;
	class OneAnimation;
}

class ModelAnimationData;
class LoadMeshData;
class ModelData;
class ModelParser;
class FBXParser;
class FBXModel;
class TextureBuffer;
class GraphicEngineManager;
class FBXManager;
class TextureManager;
class EATERManager;

class LoadManager
{
private:
	CRITICAL_SECTION* g_CS;

public:
	LoadManager();
	~LoadManager();

	//초기화 및 경로 설정
	void Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs);
	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode);
	void Start();
	int	GetMeshCount();
	int	GetTextureCount();
	int	GetAnimationCount();

	static TextureBuffer*	GetTexture(std::string Path);
	static ModelData*		GetMesh(std::string Path);
	static ModelAnimationData* GetAnimation(std::string Path);
	static bool				FindMesh(std::string Name);
	static bool				FindTexture(std::string Name);
	static int				FindInstanceIndex(int index);
private:
	bool	CheckFolder(std::string& Path);
	void	LoadFile(std::string& Path, UINT MODE);
	void	LoadFolder(std::string& Path, UINT MODE);

private:
	std::string MeshPath;
	std::string TexturePath;
private:
	static std::map<std::string, ModelData*>			ModelList;
	static std::map<std::string, TextureBuffer*>		TextureList;
	static std::map<std::string, ModelAnimationData*>	AnimationList;

	//인스턴스 인덱스 관리
	static std::vector<int> GrobalInstanceIndexList;
	static bool isNewMesh;
	static int GrobalInstanceIndex;
private:
	GraphicEngineManager*	GEngine;
	FBXManager*				mFBX;
	TextureManager*			mTexture;
	EATERManager*			mEATER;

	friend FBXManager;
	friend TextureManager;
	friend EATERManager;
};
