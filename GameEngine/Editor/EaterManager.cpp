#include "EaterManager.h"
#include "EaterHeader.h"

#include "ModelParser.h";

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
	MeshManager			= new E_MeshManager();
	AnimationManager	= new E_AnimationManager();
	MaterialManager		= new E_MaterialManager();
	BufferManager		= new E_BufferManager();
}

std::string EaterManager::CutFileName(std::string FilePath)
{
	size_t start = FilePath.rfind("/") + 1;
	size_t end = FilePath.rfind(".") - start;
	std::string FileType = FilePath.substr(start, end);
	return FileType;
}

void EaterManager::LoadEaterFile(std::string& Path)
{
	






}

void EaterManager::Load_FBX_File(std::string& Path, ParserData::Model* FBXMesh)
{
	std::string SaveFileName = Path;
	

	int MeshSize = (int)FBXMesh->m_MeshList.size();

	FBXMesh->m_MeshList[0]->m_MeshType;

	
	std::string FileName = CutFileName(SaveFileName);

	///Model 정보를 저장한다
	EATER_CREATE_FILE(FileName,"../Assets/Model/ModelData/");
	MeshManager->SetFileName(FileName);
	MeshManager->ChangeEaterFile(FBXMesh);
	EATER_CLOSE_FILE();

	///Material 정보를 저장한다
	EATER_CREATE_FILE(FileName, "../Assets/Texture/Material/",".EMAT");
	MaterialManager->SetFileName(FileName);
	MaterialManager->ChangeEaterFile(FBXMesh);
	EATER_CLOSE_FILE();

	///MeshBuffer 정보를 저장한다
	EATER_CREATE_FILE(FileName, "../Assets/Model/MeshBuffer/",".EMESH");
	BufferManager->SetFileName(FileName);
	BufferManager->ChangeEaterFile(FBXMesh);
	EATER_CLOSE_FILE();

	///Animation 정보를 저장한다
	////애니메이션 있으면 체크
	//EATER_CREATE_FILE(FileName, "../Assets/Model/StaticModel/");
	////MaterialManager->
	//EATER_CLOSE_FILE();	
}

