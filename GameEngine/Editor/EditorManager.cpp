#include "EditorManager.h"

#include "FBXManager.h"
#include "YamlManager.h"
#include "EaterManager.h"
#include "EditorData.h"

#include "GameObject.h"

EditorManager::EditorManager()
{
	mYaml	= nullptr;
	mFbx	= nullptr;
	mEater	= nullptr;
}

EditorManager::~EditorManager()
{
	delete mYaml;
	delete mFbx;
	delete mEater;
}

void EditorManager::Initialize()
{
	mYaml	= new YamlManager();
	mFbx	= new FBXManager();
	mEater	= new EaterManager();

	mEater->Initialize();
	mFbx->Initialize(mEater);

	//에셋폴더안에 필요한 폴더들을 미리 생성한다
	CreateAssetsFile();
}

void EditorManager::ConvertData(std::string Path)
{
	size_t start = Path.rfind(".")+1;
	size_t end = Path.length()- start;
	std::string FileType = Path.substr(start, end);

	if (FileType == "txt")
	{

	}
	else if (FileType == "Eater")
	{
		mEater->Load_Eater_File(Path);
	}
	else if (FileType == "fbx" || FileType == "FBX")
	{
		ParserData::CModel* Model = mFbx->OpenFile(Path);
		mEater->Load_FBX_File(Path, Model);
	}
	else if (FileType == "unity")
	{
		mYaml->OpenFile(Path);
	}
	else if (FileType == "Emat")
	{
		mEater->Load_Eater_File(Path);
	}
	else if (FileType == "Emesh")
	{
		mEater->Load_Eater_File(Path);
	}
}

void EditorManager::ConvertData(std::string Path, std::string ChangeName, CHANGE_TYPE Option)
{
	if (Option == CHANGE_TYPE::COLLIDER)
	{
		ParserData::CModel* Model = mFbx->OpenFile(Path);
		mEater->Load_FBX_File_MeshBuffer(Path, Model, ChangeName);
	}
	else if (Option == CHANGE_TYPE::NAVMESH)
	{
		ParserData::CModel* Model = mFbx->OpenFile(Path);
	}
}

void EditorManager::ConvertAnimationData(std::string Path, std::string ChangeName, float min, float max)
{
	mEater->Load_Animation_File(Path, ChangeName, min, max);
}

void EditorManager::OpenEaterFile(std::string Path,int Type)
{
	mEater->Load_Eater_File(Path);
}

void EditorManager::OpenEaterGameObject(GameObject* Object,ObjectOption* Option)
{
	mEater->Load_GameObject_File(Object, Option);
}

void EditorManager::CreateMaterialData(InstanceMaterial* m)
{
	mEater->Create_Material_File(m);
}

void EditorManager::CreateMaterialData(GameObject* Obj, std::string& FilePath)
{
	mEater->Create_Material_File(Obj, FilePath);
}

void EditorManager::CreateParticleData(GameObject* Obj, std::string& FilePath)
{
	mEater->Create_Particle_File(Obj, FilePath);
}

void EditorManager::CreateAssetsFile()
{
	//에셋폴더 만들기
	std::filesystem::create_directory("../Assets");

	//상위 폴더 만들기
	std::filesystem::create_directory("../Assets/Model");
	std::filesystem::create_directory("../Assets/Scene");
	std::filesystem::create_directory("../Assets/Texture");
	std::filesystem::create_directory("../Assets/Sound");

	//Model 폴더 생성
	std::filesystem::create_directory("../Assets/Model/Animation");
	std::filesystem::create_directory("../Assets/Model/ModelData");
	std::filesystem::create_directory("../Assets/Model/TerrainModel");
	std::filesystem::create_directory("../Assets/Model/MeshBuffer");
	std::filesystem::create_directory("../Assets/Model/Prefap");

	//텍스쳐 폴더 생성
	std::filesystem::create_directory("../Assets/Texture/Particle");
	std::filesystem::create_directory("../Assets/Texture/Graphic");
	std::filesystem::create_directory("../Assets/Texture/Environment");
	std::filesystem::create_directory("../Assets/Texture/ModelTexture");
	std::filesystem::create_directory("../Assets/Texture/Material");

	//그래픽용 폴더 생성
	std::filesystem::create_directory("../Assets/Texture/Graphic/Icon");
	std::filesystem::create_directory("../Assets/Texture/Graphic/Noise");
	std::filesystem::create_directory("../Assets/Texture/Graphic/Shader");

	CreateBaseObject();
}

void EditorManager::CreateBaseObject()
{
	bool isCopy = false;
	CopyFile(_T("..\\Editor\\Export\\MeshBuffer\\Sphere_0.Emesh"), _T("..\\Assets\\Model\\MeshBuffer\\Sphere_0.Emesh"), false);
	CopyFile(_T("..\\Editor\\Export\\MeshBuffer\\Grid_0.Emesh"), _T("..\\Assets\\Model\\MeshBuffer\\Grid_0.Emesh"), false);
	CopyFile(_T("..\\Editor\\Export\\MeshBuffer\\Monkey_0.Emesh"), _T("..\\Assets\\Model\\MeshBuffer\\Monkey_0.Emesh"), false);
	CopyFile(_T("..\\Editor\\Export\\MeshBuffer\\Cylinder_0.Emesh"), _T("..\\Assets\\Model\\MeshBuffer\\Cylinder_0.Emesh"), false);
	CopyFile(_T("..\\Editor\\Export\\MeshBuffer\\Box_0.Emesh"), _T("..\\Assets\\Model\\MeshBuffer\\Box_0.Emesh"), false);


	CopyFile(_T("..\\Editor\\Export\\ModelData\\Sphere.Eater"), _T("..\\Assets\\Model\\ModelData\\Sphere.Eater"), false);
	CopyFile(_T("..\\Editor\\Export\\ModelData\\Monkey.Eater"), _T("..\\Assets\\Model\\ModelData\\Monkey.Eater"), false);
	CopyFile(_T("..\\Editor\\Export\\ModelData\\Grid.Eater"), _T("..\\Assets\\Model\\ModelData\\Grid.Eater"), false);
	CopyFile(_T("..\\Editor\\Export\\ModelData\\Cylinder.Eater"), _T("..\\Assets\\Model\\ModelData\\Cylinder.Eater"), false);
	CopyFile(_T("..\\Editor\\Export\\ModelData\\Box.Eater"), _T("..\\Assets\\Model\\ModelData\\Box.Eater"), false);

	CopyFile(_T("..\\Editor\\Export\\Material\\Sphere.Emat"), _T("..\\Assets\\Texture\\Material\\Sphere.Emat"), false);
	CopyFile(_T("..\\Editor\\Export\\Material\\Monkey.Emat"), _T("..\\Assets\\Texture\\Material\\Monkey.Emat"), false);
	CopyFile(_T("..\\Editor\\Export\\Material\\Grid.Emat"), _T("..\\Assets\\Texture\\Material\\Grid.Emat"), false);
	CopyFile(_T("..\\Editor\\Export\\Material\\Cylinder.Emat"), _T("..\\Assets\\Texture\\Material\\Cylinder.Emat"), false);
	CopyFile(_T("..\\Editor\\Export\\Material\\Box.Emat"), _T("..\\Assets\\Texture\\Material\\Box.Emat"), false);
}

