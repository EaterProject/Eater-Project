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
#include "MeshManager.h"
#include "MaterialManager.h"
#include "AnimationManager.h"
#include "GraphicsEngine.h"
#include "GraphicEngineManager.h"
#include "GameObject.h"
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

	mMeshManager = new MeshManager();
	mMaterialManager = new MaterialManager();

	mAnimationManger = new AnimationManager();
	mAnimationManger->Initialize(graphic, _cs);

	mFBX = new FBXManager();
	mFBX->Initialize(graphic, _cs);

	mEATER = new EATERManager();
	mEATER->Initialize(graphic, _cs);

	mTexture = new TextureManager();
	mTexture->Initialize(graphic, _cs);
}

void LoadManager::Release()
{
	///�� ������ ����
	auto ModelStart	= ModelDataList.begin();
	auto ModelEnd	= ModelDataList.end();
	for (ModelStart; ModelStart != ModelEnd; ModelStart++)
	{
		ModelData* Data = ModelStart->second;
		ModelStart->second = nullptr;
		delete Data;
	}
	ModelDataList.clear();

	///�ؽ��� ���� ����
	auto TextureListStart	= TextureList.begin();
	auto TextureListEnd		= TextureList.end();
	for (TextureListStart; TextureListStart != TextureListEnd; TextureListStart++)
	{
		TextureBuffer* Data = TextureListStart->second;
		TextureListStart->second = nullptr;
		delete Data;
	}
	TextureList.clear();

	///���׸��� ����
	auto MaterialListStart	= MaterialList.begin();
	auto MaterialListEnd	= MaterialList.end();
	for (MaterialListStart; MaterialListStart != MaterialListEnd; MaterialListStart++)
	{
		Material* Data = MaterialListStart->second;
		MaterialListStart->second = nullptr;
		Data->Release();
	}
	MaterialList.clear();

	///�ִϸ��̼� ������ ����
	auto AnimationListStart = AnimationList.begin();
	auto AnimationListEnd	= AnimationList.end();
	for (AnimationListStart; AnimationListStart != AnimationListEnd; AnimationListStart++)
	{
		Animation* Data = AnimationListStart->second;
		AnimationListStart->second = nullptr;
		Data->Release();
	}
	AnimationList.clear();

	///�Ž� ������ ����
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
	//����,���� ����
	if (CheckFolder(Path) == true)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Load ][ Folder ] %s", Path.c_str());
		LoadFolder(Path, MODE);
	}
	else
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Load ][ File ] %s", Path.c_str());
		LoadFile(Path, MODE);
	}
}

void LoadManager::LoadTerrain(std::string mMeshName, std::string mMaskName1, std::string mMaskName2, UINT parsingMode)
{
	mFBX->LoadTerrain(mMeshName, mMaskName1, mMaskName2, parsingMode);
}

void LoadManager::BakeSkyLightMap(std::string Path, bool hdri)
{
	//�ؽ��� �ε�
	mTexture->BakeSkyLightMap(Path, hdri);
}

void LoadManager::BakeConvertSkyLightMap(std::string& Path, float angle, float threshold, bool hdri)
{
	mTexture->BakeConvertSkyLightMap(Path, angle, threshold, hdri);
}

void LoadManager::BakeConvertSkyCubeMap(std::string& Path, float angle, float threshold, bool hdri)
{
	mTexture->BakeConvertSkyCubeMap(Path, angle, threshold, hdri);
}

void LoadManager::BakeAnimation()
{
	mAnimationManger->BakeAnimation();
}

void LoadManager::SaveConvertSkyLightMap(std::string& Path)
{
	mTexture->SaveConvertSkyLightMap(Path);
}

void LoadManager::SaveConvertSkyCubeMap(std::string& Path)
{
	mTexture->SaveConvertSkyCubeMap(Path);
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetTexture ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetSkyLight ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetMaterial ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetMaterial ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetModel ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetMesh ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetMesh ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetMesh ] '%s'�� �����ϴ�.", Path.c_str());
		return nullptr;
	}
	else
	{
		return Find_it->second->m_AnimationBuffer;
	}

	return nullptr;
}

CameraAnimation* LoadManager::GetCamAnimation(std::string Path)
{
	std::map<std::string, CameraAnimation*>::iterator End_it = CamAnimationList.end();
	std::map<std::string, CameraAnimation*>::iterator Find_it = CamAnimationList.find(Path);
	if (End_it == Find_it)
	{
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetCamAnimation ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetCollider ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ GetAnimation ] '%s'�� �����ϴ�.", Path.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ FindModel ] '%s'�� �����ϴ�.", Name.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ FindTexture ] '%s'�� �����ϴ�.", Name.c_str());
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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ DeleteTexture ] '%s'�� �����ϴ�.", Path.c_str());
	}
	else
	{
		GraphicEngine::Get()->DeleteTexture(Find_it->second);

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
		PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ ERROR ][ Engine ][ DeleteSkyLight ] '%s'�� �����ϴ�.", Path.c_str());
	}
	else
	{
		GraphicEngine::Get()->DeleteSkyLight(Find_it->second);

		delete Find_it->second;

		SkyLightList.erase(Find_it);
	}
}

bool LoadManager::CheckFolder(std::string& Path)
{
	//��ο��� .�� ã�����ߴٸ� ����
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
	//���� üũ �ؽ���, FBX, ��ü���� ,�����Ͽ� �ε�
	//�ε��� �����ʹ� LoadManager�ȿ� ����Ʈ�� ����

	std::size_t Start	= Path.rfind('.') + 1;
	std::size_t End		= Path.length() - Start;
	std::string Type	= Path.substr(Start, End);
	
	if (Type == "png" || Type == "PNG" || Type == "dds" || Type == "DDS" || Type == "hdr" || Type == "HDR")
	{
		//�ؽ��� �ε�
		mTexture->LoadTexture(Path);
	}
	else if(Type == "fbx")
	{
		//FBX�ε�
		mFBX->Load(Path, MODE);
		BakeAnimation();
	}
	else if (Type == "Eater")
	{
		//��ü ���� �ε�
		mEATER->Load(Path, MODE);
		BakeAnimation();
	}
	else if (Type == "Scene")
	{
		//�� �ε�
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
}

void LoadManager::LoadFolder(std::string& Path, UINT MODE)
{
	//������ �ε��Ѵ� �����ȿ� �������ε� ������ ������ ���� �ٽ÷ε�

	std::filesystem::path p(Path);
	if (std::filesystem::exists(p) == false)
	{
		return;
	}

	std::filesystem::directory_iterator itr(p);
	while (itr != std::filesystem::end(itr))
	{
		const std::filesystem::directory_entry& entry = *itr;

		//���� ������ �̸��� �˾ƿ´�
		if (std::filesystem::is_directory(entry.path()) == true)
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';

			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Load ][ Folder ] %s", Path.c_str());
			LoadFolder(Path, MODE);
		}
		else
		{
			std::string Path = entry.path().string();
			std::size_t Swap = Path.rfind('\\');
			Path[Swap] = '/';

			PROFILE_LOG(PROFILE_OUTPUT::CONSOLE, "[ Engine ][ Load ][ File ] %s", Path.c_str());
			LoadFile(Path, MODE);
		}
		itr++;
	}
}