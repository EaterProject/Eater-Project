#include "LoadManager.h"

#include <filesystem>
#include <iostream>

#include "EngineData.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "FBXManager.h"
#include "TextureManager.h"
#include "EATERManager.h"
#include "AnimationManager.h"
#include "GraphicEngineAPI.h"
#include "GraphicEngineManager.h"
#include "GameObject.h"

#include "MeshManager.h"
#include "MaterialManager.h"
#include "EaterSound.h"
#include "GameEngine.h"
#include "Profiler/Profiler.h"



std::map<std::string, ModelData*>			LoadManager::ModelDataList;

std::map<std::string, TextureBuffer*>		LoadManager::TextureList;
std::map<std::string, SkyLightBuffer*>	LoadManager::SkyLightList;

std::map<std::string, Mesh*>				LoadManager::MeshBufferList;
std::map<std::string, Material*>			LoadManager::MaterialList;
std::map<std::string, Animation*>			LoadManager::AnimationList;

std::map<std::string, CameraAnimation*>		LoadManager::CamAnimationList;
std::map<std::string, ColliderBuffer*>		LoadManager::ColliderBufferList;		
std::map<std::string, GameObject*>			LoadManager::PrefapList;
std::map<std::string, LoadParticleData*>	LoadManager::ParticleList;

LoadManager::LoadManager()
{

}

LoadManager::~LoadManager()
{

}

void LoadManager::Initialize(GraphicEngineManager* graphic, CRITICAL_SECTION* _cs)
{
	g_CS = _cs;
	mGraphicManager = graphic;

	mAnimationManger = new AnimationManager();
	mAnimationManger->Initialize(graphic, _cs);

	mFBX = new FBXManager();
	mFBX->Initialize(graphic, _cs);

	mEATER = new EATERManager();
	mEATER->Initialize(graphic, _cs);

	mTexture = new TextureManager();
	mTexture->Initialize(graphic, _cs);

	mSound = EaterSound::GetInstance();
}

void LoadManager::Release()
{
	///모델 데이터 삭제
	auto ModelStart	= ModelDataList.begin();
	auto ModelEnd	= ModelDataList.end();
	for (ModelStart; ModelStart != ModelEnd; ModelStart++)
	{
		ModelData* Data = ModelStart->second;
		ModelStart->second = nullptr;
		delete Data;
	}
	ModelDataList.clear();

	///텍스쳐 버퍼 삭제
	auto TextureListStart	= TextureList.begin();
	auto TextureListEnd		= TextureList.end();
	for (TextureListStart; TextureListStart != TextureListEnd; TextureListStart++)
	{
		TextureBuffer* Data = TextureListStart->second;
		TextureListStart->second = nullptr;
		delete Data;
	}
	TextureList.clear();

	///메테리얼 삭제
	auto MaterialListStart	= MaterialList.begin();
	auto MaterialListEnd	= MaterialList.end();
	for (MaterialListStart; MaterialListStart != MaterialListEnd; MaterialListStart++)
	{
		Material* Data = MaterialListStart->second;
		MaterialListStart->second = nullptr;
		Data->Release();
	}
	MaterialList.clear();

	///애니메이션 데이터 삭제
	auto AnimationListStart = AnimationList.begin();
	auto AnimationListEnd	= AnimationList.end();
	for (AnimationListStart; AnimationListStart != AnimationListEnd; AnimationListStart++)
	{
		Animation* Data = AnimationListStart->second;
		AnimationListStart->second = nullptr;
		Data->Release();
	}
	AnimationList.clear();

	///매쉬 데이터 삭제
	auto MeshBufferStart	= MeshBufferList.begin();
	auto MeshBufferEnd		= MeshBufferList.end();
	for (MeshBufferStart; MeshBufferStart != MeshBufferEnd; MeshBufferStart++)
	{
		Mesh* Data = MeshBufferStart->second;
		MeshBufferStart->second = nullptr;
		Data->Release();
	}
	MeshBufferList.clear();
}

void LoadManager::Start()
{
	
}

void LoadManager::Load(std::string& Path, UINT MODE)
{
	//파일,폴더 구분
	if (CheckFolder(Path) == true)
	{
		PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Load ][ Folder ] %s", Path.c_str());
		LoadFolder(Path, MODE);
	}
	else
	{
		PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Load ][ File ] %s", Path.c_str());
		LoadFile(Path, MODE);
	}
}

void LoadManager::LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode)
{
	mFBX->LoadTerrain(mMeshName, mMaskName1, mMaskName2, parsingMode);
}

void LoadManager::BakeSkyLightMap(std::string Path, bool hdri)
{
	//텍스쳐 로드
	mTexture->BakeSkyLightMap(Path, hdri);
}

void LoadManager::BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri, UINT index)
{
	mTexture->BakeConvertSkyLightMap(Path, angle, threshold, hdri, index);
}

void LoadManager::BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri)
{
	mTexture->BakeConvertSkyCubeMap(Path, angle, threshold, hdri);
}

void LoadManager::BakeAnimation()
{
	mAnimationManger->BakeAnimation();
}

void LoadManager::SaveConvertSkyLightMap(std::string& Path, std::string& SaveName)
{
	mTexture->SaveConvertSkyLightMap(Path, SaveName);
}

void LoadManager::SaveConvertSkyCubeMap(std::string& Path, std::string& SaveName)
{
	mTexture->SaveConvertSkyCubeMap(Path, SaveName);
}

void LoadManager::SaveSpriteToVolumeTexture_LUT(std::string fileName, std::string saveName, UINT pixelSize)
{
	mTexture->SaveSpriteToVolumeTexture_LUT(fileName, saveName, pixelSize);
}

int LoadManager::GetMeshCount()
{
	return (int)ModelDataList.size();
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
	std::map<std::string, Animation*>::iterator Begin_it = AnimationList.begin();
	for (Begin_it; Begin_it != AnimationList.end(); Begin_it++)
	{
		Animation* Data = Begin_it->second;
		Count += Data->GetAnimationCount();
	}
	return Count;
}

int LoadManager::GetBufferCount()
{
	return (int)MeshBufferList.size();
}

TextureBuffer* LoadManager::GetTexture(std::string Path)
{
	std::map<std::string,TextureBuffer*>::iterator End_it	= TextureList.end();
	std::map<std::string,TextureBuffer*>::iterator Find_it	= TextureList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetTexture ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

SkyLightBuffer* LoadManager::GetSkyLight(std::string Path)
{
	std::map<std::string, SkyLightBuffer*>::iterator End_it = SkyLightList.end();
	std::map<std::string, SkyLightBuffer*>::iterator Find_it = SkyLightList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetSkyLight ] '%s'가 없습니다.", Path.c_str());
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
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetMaterial ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

Animation* LoadManager::GetAnimation(std::string Path)
{
	std::map<std::string, Animation*>::iterator End_it = AnimationList.end();
	std::map<std::string, Animation*>::iterator Find_it = AnimationList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetMaterial ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

ModelData* LoadManager::GetModelData(std::string Path)
{
	std::map<std::string, ModelData*>::iterator End_it	= ModelDataList.end();
	std::map<std::string, ModelData*>::iterator Find_it = ModelDataList.find(Path);
	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetModel ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

Mesh* LoadManager::GetMesh(std::string Path)
{
	std::map<std::string, Mesh*>::iterator End_it = MeshBufferList.end();
	std::map<std::string, Mesh*>::iterator Find_it = MeshBufferList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetMesh ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}

	return nullptr;
}

MeshBuffer* LoadManager::GetMeshBuffer(std::string Path)
{
	std::map<std::string, Mesh*>::iterator End_it = MeshBufferList.end();
	std::map<std::string, Mesh*>::iterator Find_it = MeshBufferList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetMesh ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second->m_MeshData;
	}

	return nullptr;
}

AnimationBuffer* LoadManager::GetAnimationBuffer(std::string Path)
{
	std::map<std::string, Animation*>::iterator End_it = AnimationList.end();
	std::map<std::string, Animation*>::iterator Find_it = AnimationList.find(Path);

	if (End_it == Find_it)
	{
		//PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetMesh ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second->m_AnimationBuffer;
	}

	return nullptr;
}

LoadParticleData* LoadManager::GetParticle(std::string Path)
{
	std::map<std::string, LoadParticleData*>::iterator End_it	= ParticleList.end();
	std::map<std::string, LoadParticleData*>::iterator Find_it  = ParticleList.find(Path);
	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetParticle ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
	return nullptr;
}

CameraAnimation* LoadManager::GetCamAnimation(std::string Path)
{
	std::map<std::string, CameraAnimation*>::iterator End_it = CamAnimationList.end();
	std::map<std::string, CameraAnimation*>::iterator Find_it = CamAnimationList.find(Path);
	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetCamAnimation ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

ColliderBuffer* LoadManager::GetColliderBuffer(std::string Path)
{
	std::map<std::string, ColliderBuffer*>::iterator End_it		= ColliderBufferList.end();
	std::map<std::string, ColliderBuffer*>::iterator Find_it	= ColliderBufferList.find(Path);

	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetCollider ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second;
	}
}

//std::vector<OneTriangle*>* LoadManager::GetNavMeshData()
//{
//	return &NavMeshData;
//}

ModelAnimationData* LoadManager::GetAnimationData(std::string Path)
{
	std::map<std::string, Animation*>::iterator End_it = AnimationList.end();
	std::map<std::string, Animation*>::iterator Find_it = AnimationList.find(Path);
	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ GetAnimation ] '%s'가 없습니다.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second->m_AnimationData;
	}
}

bool LoadManager::FindModel(std::string Name)
{
	std::map<std::string, ModelData*>::iterator End_it	= ModelDataList.end();
	std::map<std::string, ModelData*>::iterator Find_it = ModelDataList.find(Name);
	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ FindModel ] '%s'가 없습니다.", Name.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ FindTexture ] '%s'가 없습니다.", Name.c_str());
		return false;
	}
	else
	{
		return true;
	}
}

void LoadManager::DeleteTexture(std::string Path)
{
	std::map<std::string, TextureBuffer*>::iterator End_it = TextureList.end();
	std::map<std::string, TextureBuffer*>::iterator Find_it = TextureList.find(Path);

	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ DeleteTexture ] '%s'가 없습니다.", Path.c_str());
	}
	else
	{
		EnterCriticalSection(&GameEngine::g_CS);
		GraphicEngine::Get()->DeleteTexture(Find_it->second);
		LeaveCriticalSection(&GameEngine::g_CS);

		delete Find_it->second;

		TextureList.erase(Find_it);
	}
}

void LoadManager::DeleteSkyLight(std::string Path)
{
	std::map<std::string, SkyLightBuffer*>::iterator End_it = SkyLightList.end();
	std::map<std::string, SkyLightBuffer*>::iterator Find_it = SkyLightList.find(Path);

	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::LOG_FILE, "[ ERROR ][ Engine ][ DeleteSkyLight ] '%s'가 없습니다.", Path.c_str());
	}
	else
	{
		EnterCriticalSection(&GameEngine::g_CS);
		GraphicEngine::Get()->DeleteSkyLight(Find_it->second);
		LeaveCriticalSection(&GameEngine::g_CS);

		delete Find_it->second;

		SkyLightList.erase(Find_it);
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
	Sleep(0);

	std::size_t Start	= Path.rfind('.') + 1;
	std::size_t End		= Path.length() - Start;
	std::string Type	= Path.substr(Start, End);
	
	if (Type == "png" || Type == "PNG" || Type == "dds" || Type == "DDS" || Type == "hdr" || Type == "HDR")
	{
		//텍스쳐 로드
		mTexture->LoadTexture(Path);
	}
	else if(Type == "fbx" || Type == "FBX")
	{
		//FBX로드
		mFBX->Load(Path, MODE);
		BakeAnimation();
	}
	else if (Type == "Eater")
	{
		//자체 포멧 로드
		mEATER->Load(Path, MODE);
		BakeAnimation();
	}
	else if (Type == "Scene")
	{
		//씬 로드
		mEATER->LoadScene(Path);
	}
	else if (Type == "Emat")
	{
		mEATER->LoadMaterial(Path);
	}
	else if (Type == "Emesh")
	{
		mEATER->LoadMesh(Path);
		BakeAnimation();
	}
	else if (Type == "Prefap")
	{
		mEATER->LoadPrefap(Path);
	}
	else if (Type == "mp3" || Type == "wav")
	{
		std::size_t Start		= Path.rfind('/') + 1;
		std::size_t End			= Path.rfind('.') - Start;
		std::string FileName	= Path.substr(Start, End);

		Start	= 0;
		End		= Path.rfind('/')+1;
		std::string FilePath	= Path.substr(Start, End);


		if (Path.rfind("BGM") == std::string::npos)
		{
			mSound->SetSoundFolderPath(Sound_Category::SFX, FilePath);
			mSound->LoadSound(Sound_Category::SFX, FileName, FileName + "." + Type, false);
		}
		else
		{
			mSound->SetSoundFolderPath(Sound_Category::BGM, FilePath);
			mSound->LoadSound(Sound_Category::BGM, FileName, FileName+"."+Type,true);
		}
	}
	else if (Type == "Particle")
	{
		mEATER->LoadParticle(Path);
	}
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

			PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Load ][ Folder ] %s", Path.c_str());
			LoadFolder(Path, MODE);
		}
		else
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';

			PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "[ Engine ][ Load ][ File ] %s", Path.c_str());
			LoadFile(Path, MODE);
		}
		itr++;
	}
}