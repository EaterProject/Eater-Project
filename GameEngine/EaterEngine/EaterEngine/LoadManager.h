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

class GameObject;
class CameraAnimation;
class ColliderBuffer;
class ModelAnimationData;
class ModelData;

class MeshBuffer;
class TextureBuffer;
class AnimationBuffer;
class SkyLightBuffer;

class Mesh;
class Material;
class Animation;
class CameraAnimation;

class GraphicEngineManager;
class MeshManager;
class MaterialManager;
class AnimationManager;

class FBXManager;
class TextureManager;
class EATERManager;
class Eater_LoadAnimation;
class Eater_LoadBuffer;
class Eater_LoadMaterial;
class Eater_LoadMesh;
class Eater_LoadCamera;
class Eater_LoadScene;

class LoadManager
{
private:
	CRITICAL_SECTION* g_CS;

public:
	LoadManager();
	~LoadManager();

	//초기화 및 경로 설정
	void Initialize(GraphicEngineManager* graphic, CRITICAL_SECTION* _cs);
	void Start();
	void Release();

public:
	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode);

public:
	void BakeSkyLightMap(std::string Path, bool hdri);
	void BakeConvertCubeMap(std::string& Path, float angle, bool save_file, bool hdri, bool apply_skylight, bool apply_environment);
	void BakeAnimation();

public:
	int	GetMeshCount();
	int	GetTextureCount();
	int	GetMaterialCount();
	int	GetAnimationCount();
	int GetBufferCount();

	static ModelData*			GetModelData(std::string Path);			//모델 데이터를 가져옴
	static ModelAnimationData*	GetAnimationData(std::string Path);		//애니메이션 데이터를 가져옴

	static TextureBuffer*		GetTexture(std::string Path);			//텍스쳐 버퍼를 가져옴
	static SkyLightBuffer*		GetSkyLight(std::string Path);			//환경맵 버퍼를 가져옴
	static MeshBuffer*			GetMeshBuffer(std::string Path);		//메쉬 버퍼 데이터를 가져옴
	static AnimationBuffer*		GetAnimationBuffer(std::string Path);	//애니메이션 버퍼 데이터를 가져옴

	static Mesh*				GetMesh(std::string Path);				//메쉬를 가져옴
	static Material*			GetMaterial(std::string Path);			//메테리얼을 가져옴
	static Animation*			GetAnimation(std::string Path);			//애니메이션을 가져옴

	static ColliderBuffer*		GetColliderBuffer(std::string Path);	//카메라 애니메이션 데이터를가져옴
	static CameraAnimation*		GetCamAnimation(std::string Path);		//카메라 애니메이션 데이터를가져옴
	
	static bool					FindModel(std::string Name);			
	static bool					FindTexture(std::string Name);

	static void					DeleteTexture(std::string Path);
	static void					DeleteSkyLight(std::string Path);

private:
	bool	CheckFolder(std::string& Path);
	void	LoadFile(std::string& Path, UINT MODE);
	void	LoadFolder(std::string& Path, UINT MODE);

private:
	std::string MeshPath;
	std::string TexturePath;

private:
	static std::map<std::string, ModelData*>			ModelDataList;

	static std::map<std::string, TextureBuffer*>		TextureList;
	static std::map<std::string, SkyLightBuffer*>		SkyLightList;

	static std::map<std::string, Mesh*>					MeshBufferList;
	static std::map<std::string, Material*>				MaterialList;
	static std::map<std::string, Animation*>			AnimationList;

	static std::map<std::string, CameraAnimation*>		CamAnimationList;
	static std::map<std::string, ColliderBuffer*>		ColliderBufferList;
	static std::map<std::string, GameObject*>			PrefapList;

private:
	FBXManager*				mFBX;
	TextureManager*			mTexture;
	EATERManager*			mEATER;

	MeshManager*			mMeshManager;		// Mesh 관리 매니저
	MaterialManager*		mMaterialManager;	// Material 관리 매니저
	AnimationManager*		mAnimationManger;	// Animation 관리 매니저

	GraphicEngineManager*	mGraphicManager;	// Graphic 관리 매니저

	friend FBXManager;
	friend TextureManager;
	friend AnimationManager;
	friend EATERManager;

	friend Eater_LoadScene;
	friend Eater_LoadAnimation;
	friend Eater_LoadBuffer;
	friend Eater_LoadMaterial;
	friend Eater_LoadMesh;
	friend Eater_LoadCamera;
};
