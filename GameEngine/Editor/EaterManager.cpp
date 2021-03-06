#include "EaterManager.h"
#include "EaterHeader.h"

#include "ModelParser.h"

#include "GameObject.h"
#include "MeshFilter.h"
#include "AnimationController.h"

#include "E_MaterialManager.h"
#include "E_MeshManager.h"
#include "E_AnimationManager.h"
#include "E_BufferManager.h"
#include "E_ChangeManager.h"
#include "E_ParticleManager.h"

#include "EditorToolScene.h"
#include "EditorData.h"


EaterManager::EaterManager()
{
	mMeshManager		= nullptr;
	mAnimationManager	= nullptr;
	mMaterialManager	= nullptr;
	mBufferManager		= nullptr;
	mChangeManager		= nullptr;
	mParticleManager	= nullptr;
}

EaterManager::~EaterManager()
{
	delete mMeshManager;
	delete mAnimationManager;
	delete mMaterialManager;
	delete mBufferManager;
	delete mChangeManager;
	delete mParticleManager;
}

void EaterManager::Initialize()
{
	mMeshManager		= new E_MeshManager();
	mAnimationManager	= new E_AnimationManager();
	mMaterialManager	= new E_MaterialManager();
	mBufferManager		= new E_BufferManager();
	mChangeManager		= new E_ChangeManager();
	mParticleManager	= new E_ParticleManager();

	mParticleManager->Initialize();

}

std::string EaterManager::CutFileName(std::string FilePath)
{
	size_t start	= FilePath.rfind("/") + 1;
	size_t end		= FilePath.rfind(".") - start;
	std::string FileType = FilePath.substr(start, end);

	if (FileType.rfind('+') != std::string::npos)
	{
		isSkin			= true;
		SkinName		= FileType.substr(0, FileType.rfind('+'));
		AnimationName	= FileType.substr(FileType.rfind('+'), FileType.length());
	}
	return FileType;
}

int EaterManager::CheckModelType(ParserData::CModel* FBXModel)
{
	int TYPE = E_STATIC_MESH;
	int BoneCount = FBXModel->m_BoneCount;
	int StaticCount = FBXModel->m_StaticCount;
	int SkinCount = FBXModel->m_SkinCount;

	if (SkinCount > 0)
	{
		TYPE = E_SKIN_MESH;
	}
	return TYPE;
}

void EaterManager::Load_Eater_File(std::string& Path)
{

	

}

void EaterManager::Load_FBX_File(std::string& Path, ParserData::CModel* FBXMesh)
{
	std::string FileName = CutFileName(Path);
	std::string AnimationName = FileName;
	int ModelType = CheckModelType(FBXMesh);

	//???? ???????????? ?????? ????????????
	if (ModelType == E_SKIN_MESH)
	{
		std::size_t Start = FileName.rfind('+') + 1;
		std::size_t End = FileName.length() - Start;
		FileName = FileName.substr(0, FileName.rfind('+') + 1);
	}

	///Model ?????? ????????
	EATER_OPEN_WRITE_FILE(FileName, "../Assets/Model/ModelData/", ".Eater");
	mMeshManager->SetFileName(FileName);
	mMeshManager->ChangeEaterFile(FBXMesh);
	EATER_CLOSE_WRITE_FILE();

	std::string LoadPath = "../Assets/Model/ModelData/" + FileName + ".Eater";

	EditorToolScene::MeshLoad(LoadPath);

	///MeshBuffer ?????? ????????
	mBufferManager->SetFileName(FileName);
	mBufferManager->ChangeEaterFile(FBXMesh);

	///Animation ?????? ????????
	mAnimationManager->SetFileName(AnimationName);
	mAnimationManager->ChangeEaterFile(FBXMesh);

	///Material ?????? ????????
	mMaterialManager->SetFileName(FileName);
	mMaterialManager->ChangeEaterFile(FBXMesh);
}

void EaterManager::Load_FBX_File_MeshBuffer(std::string& Path, ParserData::CModel* FBXMesh, std::string ChangeFileName)
{
	mBufferManager->SetFileName(Path);
	mBufferManager->ChangeEaterFile_Pos(FBXMesh, ChangeFileName);
	//Demo::MeshLoad(FileName);
}

void EaterManager::Load_GameObject_File(GameObject* Object, ObjectOption* mOption)
{
	//?????????? ?????? ????????
	MeshFilter* MF = Object->GetComponent<MeshFilter>();
	AnimationController* AC = Object->GetComponent<AnimationController>();

	if (MF == nullptr) { return; }

	std::string ModelName = MF->GetModelName();
	std::string MaterialName = MF->GetMaterialName();


	//?????? ?????? ?????? ????
	std::string ModelPath = "../Assets/Model/ModelData/" + ModelName + ".Eater";
	std::string MaterialPath = "../Assets/Texture/Material/" + MaterialName + ".Emat";
	mChangeManager->SetModelName(ModelName);

	//???? ?????? ???? ?? ???? ????
	EATER_OPEN_READ_FILE(ModelPath);
	int NodeCount = EATER_GET_NODE_COUNT();
	for (int i = 0; i < NodeCount; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "STATIC")
		{
			mChangeManager->Change_Static(i, Object);
		}
		else if (NodeName == "SKIN")
		{
			mChangeManager->Change_Skin(i, Object);
		}
		else if (NodeName == "BONE")
		{
			mChangeManager->Change_Bone(i, Object);
		}
	}
	EATER_CLOSE_CHANGE_FILE(ModelName, "../Assets/Model/ModelData/", ".Eater");
	//?????? ?????? ????????????
	EditorToolScene::MeshLoad(ModelPath);

	//???????? ?????? ????????
	EATER_OPEN_READ_FILE(MaterialPath);
	NodeCount = EATER_GET_NODE_COUNT();
	for (int i = 0; i < NodeCount; i++)
	{
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "EATERMAT")
		{
			mChangeManager->Change_Material(i, Object);
		}
	}
	//EATER_CLOSE_CHANGE_FILE(MaterialName, "../Assets/Texture/Material/", ".Emat");

	//if (AC != nullptr)
	//{
	//	//?????????? ?????? ????????
	//	EATER_OPEN_READ_FILE("");
	//
	//	EATER_CLOSE_READ_FILE();
	//}
}

void EaterManager::Load_Animation_File(std::string& FileName,std::string& ChangeName, float& min, float& max)
{
	mAnimationManager->ChangeEaterFile(FileName, ChangeName,min,max);
}

void EaterManager::Create_Material_File(GameObject* Object, std::string& FilePath)
{
	mMaterialManager->SetFileName(FilePath);
	mMaterialManager->CreateEaterFile(Object);
}

void EaterManager::Create_Particle_File(GameObject* Object, std::string& FilePath)
{
	mParticleManager->SetFileName(FilePath);
	mParticleManager->CreateEaterFile(Object);
}

void EaterManager::Create_Material_File(InstanceMaterial* m)
{
	mMaterialManager->Create(m);
}

