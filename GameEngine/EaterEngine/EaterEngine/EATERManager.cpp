#include "EATERManager.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "GraphicEngineManager.h"
#include "EaterEngineAPI.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"
#include "Material.h"
#include "Mesh.h"


#include "Eater_LoadBuffer.h"
#include "Eater_LoadMaterial.h"
#include "Eater_LoadMesh.h"
#include "Eater_LoadScene.h"

EATERManager::EATERManager()
{
	m_Graphic = nullptr;
	m_CriticalSection = nullptr;

	mEaterScene		= nullptr;
	mEaterBuffer	= nullptr;
	mEaterMaterial	= nullptr;
	mEaterMesh		= nullptr;
}

EATERManager::~EATERManager()
{
	m_Graphic = nullptr;
	m_CriticalSection = nullptr;
}

void EATERManager::Initialize(GraphicEngineManager* Graphic, CRITICAL_SECTION* _cs)
{
	m_Graphic = Graphic;
	m_CriticalSection = _cs;

	mEaterBuffer	= new Eater_LoadBuffer();
	mEaterMaterial	= new Eater_LoadMaterial();
	mEaterMesh		= new Eater_LoadMesh();
	mEaterScene		= new Eater_LoadScene();

	mEaterMaterial->Initialize();
	mEaterMesh->Initialize(Graphic, _cs);
	mEaterScene;
	mEaterBuffer->Initialize(Graphic, _cs);
}

void EATERManager::Load(std::string& Path, UINT parsingMode)
{
	//Eater파일로드
	mEaterMesh->LoadData(Path);
}

void EATERManager::LoadScene(std::string& Path)
{
	//씬파일 로드
	mEaterScene->LoadData(Path);
}

void EATERManager::LoadMesh(std::string& Path)
{
	//버퍼 파일로드
	mEaterBuffer->LoadData(Path);
}

void EATERManager::LoadMaterial(std::string& Path)
{
	//메테리얼 파일로드
	mEaterMaterial->LoadData(Path);
}

void EATERManager::LoadPrefap(std::string& path)
{
	mEaterScene->LoadPrefap(path);
}



