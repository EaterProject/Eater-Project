#include "FBXManager.h"
#include "ModelParser.h"
#include "ParserData.h"
#include "TypeOptionHeader.h"
#include "EaterHeader.h"
#include "EaterSaveData.h"
#include "EaterEngineAPI.h"
#include "EditorData.h"
#include "EATERManager.h"

FBXManager::FBXManager()
{
	
}

FBXManager::~FBXManager()
{
	FbxFactory->Release();
}

void FBXManager::Initialize(EaterManager* mManager)
{
	//FBX를 로드할 펙토리 생성
	FbxFactory = ModelParser::Create(FBX_MODEL);
	FbxFactory->Initialize();
}

ParserData::CModel* FBXManager::OpenFile(std::string& Path,UINT Option)
{
	ParserData::CModel* mMesh = FbxFactory->LoadModel(Path, Option);
	return mMesh;
}

