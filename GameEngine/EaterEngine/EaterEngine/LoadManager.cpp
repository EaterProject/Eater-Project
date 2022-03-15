#include "LoadManager.h"

#include <filesystem>
#include <iostream>

#include "EngineData.h"
#include "Material.h"
#include "FBXManager.h"
#include "TextureManager.h"
#include "EATERManager.h"
#include "MaterialManager.h"


std::map<std::string, ModelData*>			LoadManager::ModelList;
std::map<std::string, TextureBuffer*>		LoadManager::TextureList;
std::map<std::string, Material*>			LoadManager::MaterialList;
std::map<std::string, ModelAnimationData*>	LoadManager::AnimationList;
std::vector<int>							LoadManager::GrobalInstanceIndexList;

bool LoadManager::isNewMesh = false;
int LoadManager::GrobalInstanceIndex= 1;
LoadManager::LoadManager()
{

}

LoadManager::~LoadManager()
{

}

void LoadManager::Initialize(GraphicEngineManager* graphic, MaterialManager* material, CRITICAL_SECTION* _cs)
{
	g_CS = _cs;

	mMaterial = material;

	mFBX = new FBXManager();
	mFBX->Initialize(graphic, _cs);

	mEATER = new EATERManager();
	mEATER->Initialize(graphic, _cs);

	mTexture = new TextureManager();
	mTexture->Initialize(graphic, _cs);

	GrobalInstanceIndexList.push_back(0);
}

void LoadManager::Start()
{
	
}

void LoadManager::Load(std::string& Path, UINT MODE)
{
	//파일,폴더 구분
	if (CheckFolder(Path) == true)
	{
		//폴더 경로가 들어왔다면 그파일부터 모든 파일들을 다읽음 (폴더안에 폴더까지)
		LoadFolder(Path, MODE);
	}
	else
	{
		//파일경로가 들어왔다면 파일을 읽음
		LoadFile(Path, MODE);
	}
}

void LoadManager::LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode)
{
	isNewMesh = true;
	mFBX->LoadTerrain(mMeshName, mMaskName, parsingMode);
}

int LoadManager::GetMeshCount()
{
	return (int)ModelList.size();
}

int LoadManager::GetTextureCount()
{
	return (int)TextureList.size();
}

int LoadManager::GetMaterialCount()
{
	return (int)MaterialList.size();
}

int LoadManager::GetAnimationCount()
{
	int Count = 0;
	std::map<std::string, ModelAnimationData*>::iterator Begin_it = AnimationList.begin();
	for (Begin_it; Begin_it != AnimationList.end(); Begin_it++)
	{
		ModelAnimationData* Data = Begin_it->second;
		Count += (int)Data->AnimList.size();
	}
	return Count;
}

TextureBuffer* LoadManager::GetTexture(std::string Path)
{
	std::map<std::string,TextureBuffer*>::iterator End_it	= TextureList.end();
	std::map<std::string,TextureBuffer*>::iterator Find_it	= TextureList.find(Path);

	if (End_it == Find_it)
	{
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

Material* LoadManager::GetMaterial(std::string Path)
{
	std::map<std::string, Material*>::iterator End_it = MaterialList.end();
	std::map<std::string, Material*>::iterator Find_it = MaterialList.find(Path);

	if (End_it == Find_it)
	{
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

ModelData* LoadManager::GetMesh(std::string Path)
{
	std::map<std::string, ModelData*>::iterator End_it	= ModelList.end();
	std::map<std::string, ModelData*>::iterator Find_it = ModelList.find(Path);
	if (End_it == Find_it)
	{
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

ModelAnimationData* LoadManager::GetAnimation(std::string Path)
{
	std::map<std::string, ModelAnimationData*>::iterator End_it = AnimationList.end();
	std::map<std::string, ModelAnimationData*>::iterator Find_it = AnimationList.find(Path);
	if (End_it == Find_it)
	{
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

bool LoadManager::FindMesh(std::string Name)
{
	std::map<std::string, ModelData*>::iterator End_it	= ModelList.end();
	std::map<std::string, ModelData*>::iterator Find_it = ModelList.find(Name);
	if (End_it == Find_it)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool LoadManager::FindTexture(std::string Name)
{
	std::map<std::string, TextureBuffer*>::iterator End_it = TextureList.end();
	std::map<std::string, TextureBuffer*>::iterator Find_it = TextureList.find(Name);
	if (End_it == Find_it)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool LoadManager::CheckFolder(std::string& Path)
{
	//경로에서 .을 찾지못했다면 폴더
	std::size_t Chick = Path.rfind('.');
	int length = (int)Path.length();
	if (Chick < 2 || Chick == std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LoadManager::LoadFile(std::string& Path, UINT MODE)
{
	//파일 체크 텍스쳐, FBX, 자체포멧 ,구분하여 로드
	//로드한 데이터는 LoadManager안에 리스트로 담긴다

	std::size_t Start	= Path.rfind('.') + 1;
	std::size_t End		= Path.length() - Start;
	std::string Type	= Path.substr(Start, End);
	
	if (Type == "png" || Type == "dds")
	{
		//텍스쳐 로드
		mTexture->LoadTexture(Path);
	}
	else if(Type == "fbx")
	{
		//FBX로드
		isNewMesh = true;
		mFBX->Load(Path, MODE);
	}
	else if (Type == "Eater")
	{
		//자체 포멧 로드
		isNewMesh = true;
		mEATER->Load(Path, MODE);
	}
	else if (Type == "Scene")
	{
		//씬 로드
		mEATER->LoadScene(Path);
	}

	/// Material Load 추가해야함
}

void LoadManager::LoadFolder(std::string& Path, UINT MODE)
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
			LoadFolder(Path, MODE);
		}
		else
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';
			LoadFile(Path, MODE);
		}
		itr++;
	}
}

int LoadManager::FindInstanceIndex(int index)
{
	//이함수에서만 사용할 스테틱 인덱스
	static int Nowindex = 0;
	
	//새로운 매쉬가 들어왔다면 시작부분을 재설정
	if (isNewMesh == true)
	{
		Nowindex = GrobalInstanceIndexList.size();
		isNewMesh = false;
	}

	//들어온 인덱스를 글로벌 인덱스로 변환
	index += Nowindex;

	int Size = (int)GrobalInstanceIndexList.size();
	for (int i = 0; i < Size; i++)
	{
		//해당값에 인덱스가 있다면 
		if (GrobalInstanceIndexList[i] == index)
		{
			return index;
		}
	}

	GrobalInstanceIndexList.push_back(index);
	return index;
}











