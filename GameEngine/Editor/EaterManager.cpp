#include "EaterManager.h"
#include "EaterHeader.h"

#include "ModelParser.h";

#include "GameObject.h"
#include "MeshFilter.h"

#include "E_MaterialManager.h"
#include "E_MeshManager.h"
#include "E_AnimationManager.h"
#include "E_BufferManager.h"

EaterManager::EaterManager()
{


}

EaterManager::~EaterManager()
{



}

void EaterManager::Initialize()
{
	mMeshManager		= new E_MeshManager();
	mAnimationManager	= new E_AnimationManager();
	mMaterialManager	= new E_MaterialManager();
	mBufferManager		= new E_BufferManager();
}

std::string EaterManager::CutFileName(std::string FilePath)
{
	size_t start = FilePath.rfind("/") + 1;
	size_t end = FilePath.rfind(".") - start;
	std::string FileType = FilePath.substr(start, end);
	return FileType;
}

void EaterManager::Load_Eater_File(std::string& Path)
{
	







}

void EaterManager::Load_FBX_File(std::string& Path, ParserData::CModel* FBXMesh)
{
	std::string FileName = CutFileName(Path);

	///Model 정보를 저장한다
	EATER_CREATE_FILE(FileName,"../Assets/Model/ModelData/",".Eater");
	mMeshManager->SetFileName(FileName);
	mMeshManager->ChangeEaterFile(FBXMesh);
	EATER_CLOSE_FILE();

	///Material 정보를 저장한다
	mMaterialManager->SetFileName(FileName);
	mMaterialManager->ChangeEaterFile(FBXMesh);

	///MeshBuffer 정보를 저장한다
	mBufferManager->SetFileName(FileName);
	mBufferManager->ChangeEaterFile(FBXMesh);

	///Animation 정보를 저장한다
	mAnimationManager->SetFileName(FileName);
	mAnimationManager->ChangeEaterFile(FBXMesh);
}

void EaterManager::Load_GameObject_File(GameObject* Object)
{
	MeshFilter* MF = Object->GetComponent<MeshFilter>();
	int num = 0;

}

