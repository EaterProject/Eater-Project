#pragma once
#include <string>

namespace ParserData
{
	class CModel;
	class CMesh;
}

class E_MaterialManager;
class E_MeshManager;
class E_AnimationManager;
class E_BufferManager;
class E_ChangeManager;
class GameObject;
struct InstanceMaterial;
struct ObjectOption;

namespace ParserData
{
	class CModel;
}



class EaterManager
{
public:
	EaterManager();
	~EaterManager();

	void Load_Eater_File(std::string& Path);
	void Load_FBX_File(std::string& Path, ParserData::CModel* FBXMesh);
	void Load_FBX_File_MeshBuffer(std::string& Path, ParserData::CModel* FBXMesh, std::string ChangeFileName);
	void Load_FBX_File_NavMeshBuffer(std::string& Path, ParserData::CModel* FBXMesh, std::string ChangeFileName);


	void Load_GameObject_File(GameObject* Object, ObjectOption* mOption);
	void Load_Animation_File(std::string& FileName, std::string& ChangeName,float& min,float& max);
	void Create_Material(InstanceMaterial* m);

	void CreateBaseObject();
	void Initialize();
private:
	static const int E_STATIC_MESH = 0;
	static const int E_SKIN_MESH = 1;
	static const int E_ANIME_MESH = 2;
	static const int E_MAT_MESH = 3;
private:
	std::string CutFileName(std::string FilePath);
	int CheckModelType(ParserData::CModel* FBXModel);
	bool isSkin = false;
	std::string SkinName;
	std::string AnimationName;

	//FBX를 로드해서 Eater파일로 변환
	E_MeshManager*		mMeshManager;
	E_AnimationManager* mAnimationManager;
	E_MaterialManager*	mMaterialManager;
	E_BufferManager*	mBufferManager;
	E_ChangeManager*	mChangeManager;
};

