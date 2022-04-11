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

class CameraAnimation;
class ColliderBuffer;
class ModelAnimationData;
class LoadMeshData;
class ModelData;
class ModelParser;
class FBXParser;
class FBXModel;
class TextureBuffer;
class Material;
class Mesh;
class MeshBuffer;

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
class Eater_LoadCamera;

class LoadManager
{
private:
	CRITICAL_SECTION* g_CS;

public:
	LoadManager();
	~LoadManager();

	//�ʱ�ȭ �� ��� ����
	void Initialize(GraphicEngineManager* graphic, CRITICAL_SECTION* _cs);
	void Release();

	void Load(std::string& Path, UINT MODE);
	void LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode);
	void Start();
	int	GetMeshCount();
	int	GetTextureCount();
	int	GetMaterialCount();
	int	GetAnimationCount();

	static TextureBuffer*	GetTexture(std::string Path);			//�ؽ��� ���۸� ������
	static ModelData*		GetModel(std::string Path);				//�� �����͸� ������
	static ModelAnimationData* GetAnimation(std::string Path);		//�ִϸ��̼� �����͸� ������
	static Material*		GetMaterial(std::string Path);			//���׸��� �����͸� ������
	static Mesh*			GetMesh(std::string Path);				//�޽� �����͸� ������
	static MeshBuffer*		GetMeshBuffer(std::string Path);		//�޽� ���� �����͸� ������
	static CameraAnimation* GetCamAnimation(std::string Path);		//ī�޶� �ִϸ��̼� �����͸�������
	static ColliderBuffer*	GetColliderBuffer(std::string Path);	//ī�޶� �ִϸ��̼� �����͸�������
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
	static std::map<std::string, CameraAnimation*>		CamAnimationList;
	static std::map<std::string, ColliderBuffer*>		ColliderBufferList;
private:
	FBXManager*				mFBX;
	TextureManager*			mTexture;
	EATERManager*			mEATER;

	MeshManager*			mMeshManager;		// Mesh ���� �Ŵ���
	MaterialManager*		mMaterialManager;	// Material ���� �Ŵ���

	friend FBXManager;
	friend TextureManager;
	friend EATERManager;

	friend Eater_LoadAnimation;
	friend Eater_LoadBuffer;
	friend Eater_LoadMaterial;
	friend Eater_LoadMesh;
	friend Eater_LoadCamera;
};
