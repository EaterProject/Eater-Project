#pragma once

/// <summary>
/// �Ž�,�ؽ���,������,��� �׸������ؼ� �ʿ��Ѱ͵��� �ε�
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

	//�ʱ�ȭ �� ��� ����
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

	static ModelData*			GetModelData(std::string Path);			//�� �����͸� ������
	static ModelAnimationData*	GetAnimationData(std::string Path);		//�ִϸ��̼� �����͸� ������

	static TextureBuffer*		GetTexture(std::string Path);			//�ؽ��� ���۸� ������
	static SkyLightBuffer*		GetSkyLight(std::string Path);			//ȯ��� ���۸� ������
	static MeshBuffer*			GetMeshBuffer(std::string Path);		//�޽� ���� �����͸� ������
	static AnimationBuffer*		GetAnimationBuffer(std::string Path);	//�ִϸ��̼� ���� �����͸� ������

	static Mesh*				GetMesh(std::string Path);				//�޽��� ������
	static Material*			GetMaterial(std::string Path);			//���׸����� ������
	static Animation*			GetAnimation(std::string Path);			//�ִϸ��̼��� ������

	static ColliderBuffer*		GetColliderBuffer(std::string Path);	//ī�޶� �ִϸ��̼� �����͸�������
	static CameraAnimation*		GetCamAnimation(std::string Path);		//ī�޶� �ִϸ��̼� �����͸�������
	
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

	MeshManager*			mMeshManager;		// Mesh ���� �Ŵ���
	MaterialManager*		mMaterialManager;	// Material ���� �Ŵ���
	AnimationManager*		mAnimationManger;	// Animation ���� �Ŵ���

	GraphicEngineManager*	mGraphicManager;	// Graphic ���� �Ŵ���

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
