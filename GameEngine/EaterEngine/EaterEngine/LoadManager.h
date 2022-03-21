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
	class CModel;
	class CMesh;
	class CAnimation;
}

class ModelAnimationData;
class LoadMeshData;
class ModelData;
class ModelParser;
class FBXParser;
class FBXModel;
class TextureBuffer;
class Material;
class Mesh;

class GraphicEngineManager;
class MeshManager;
class MaterialManager;
class FBXManager;
class TextureManager;
class EATERManager;
class Eater_LoadAnimation;
class Eater_LoadBuffer;
class Eater_LoadMaterial;
class Eater_LoadMesh;

class LoadManager
{
private:
	CRITICAL_SECTION* g_CS;

public:
	LoadManager();
	~LoadManager();

	//초기화 및 경로 설정
	void Initialize(GraphicEngineManager* graphic, CRITICAL_SECTION* _cs);
	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode);
	void Start();
	int	GetMeshCount();
	int	GetTextureCount();
	int	GetMaterialCount();
	int	GetAnimationCount();

	static TextureBuffer*	GetTexture(std::string Path);
	static ModelData*		GetModel(std::string Path);
	static ModelAnimationData* GetAnimation(std::string Path);
	static Material*		GetMaterial(std::string Path);
	static Mesh*			GetMesh(std::string Path);
	static bool				FindModel(std::string Name);
	static bool				FindTexture(std::string Name);

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
	static std::map<std::string, Material*>				MaterialList;
	static std::map<std::string, ModelAnimationData*>	AnimationList;
	static std::map<std::string, Mesh*>					MeshBufferList;

private:
	FBXManager*				mFBX;
	TextureManager*			mTexture;
	EATERManager*			mEATER;

	MeshManager*			mMeshManager;		// Mesh 관리 매니저
	MaterialManager*		mMaterialManager;	// Material 관리 매니저

	friend FBXManager;
	friend TextureManager;
	friend EATERManager;

	friend Eater_LoadAnimation;
	friend Eater_LoadBuffer;
	friend Eater_LoadMaterial;
	friend Eater_LoadMesh;
};
