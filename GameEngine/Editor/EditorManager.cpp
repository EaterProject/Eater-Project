#include "EditorManager.h"

#include "FBXManager.h"
#include "YamlManager.h"
#include "EaterManager.h"
#include "EditorData.h"



EditorManager::EditorManager()
{
	mYaml	= nullptr;
	mFbx	= nullptr;
}

EditorManager::~EditorManager()
{
	delete mYaml;
	delete mFbx;
}

void EditorManager::Initialize()
{
	mYaml	= new YamlManager();
	mFbx	= new FBXManager();
	mEater = new EaterManager();
	mEater->Initialize();
	mFbx->Initialize(mEater);

	//에셋폴더안에 필요한 폴더들을 미리 생성한다
	CreateAssetsFile();
}

void EditorManager::SetPath(std::string Path)
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
	else if (FileType == "fbx")
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

void EditorManager::OpenEaterFile(std::string Path,int Type)
{
	mEater->Load_Eater_File(Path);
}

void EditorManager::CreateAssetsFile()
{
	//에셋폴더 만들기
	std::filesystem::create_directory("../Assets");

	//상위 폴더 만들기
	std::filesystem::create_directory("../Assets/Model");
	std::filesystem::create_directory("../Assets/Scene");
	std::filesystem::create_directory("../Assets/Texture");

	//Model 폴더 생성
	std::filesystem::create_directory("../Assets/Model/Animation");
	std::filesystem::create_directory("../Assets/Model/ModelData");
	std::filesystem::create_directory("../Assets/Model/TerrainModel");
	std::filesystem::create_directory("../Assets/Model/MeshBuffer");

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
}

void EditorManager::LoadAssets()
{
	std::string FBXPath = "../Assets/FBX";
	LoadFolder(FBXPath);
}


void EditorManager::LoadFolder(std::string& Path)
{
	//폴더를 로드한다 폴더안에 파일을로드 폴더면 안으로 들어가서 다시로드

	std::filesystem::path p(Path);
	if (std::filesystem::exists(p) == false)
	{
		return;
	}

	std::filesystem::directory_iterator itr(p);
	while (itr != std::filesystem::end(itr))
	{
		const std::filesystem::directory_entry& entry = *itr;

		//읽을 파일의 이름을 알아온다
		if (std::filesystem::is_directory(entry.path()) == true)
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';
			LoadFolder(Path);
		}
		else
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';
			LoadFile(Path);
		}
		itr++;
	}
}

void EditorManager::LoadFile(std::string& Path)
{
	size_t start = Path.rfind(".") + 1;
	size_t end = Path.length() - start;
	std::string FileType = Path.substr(start, end);

	if (FileType == "fbx" || FileType == "FBX")
	{
		mFbx->OpenFile(Path);
	}
}