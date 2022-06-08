#include "E_MaterialManager.h"
#include "ParserData.h"
#include "EaterHeader.h"
#include "EditorToolScene.h"
#include "EaterSaveData.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Material.h"

E_MaterialManager::E_MaterialManager()
{

}

E_MaterialManager::~E_MaterialManager()
{

}

void E_MaterialManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	int MeshSize = (int)FBXMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::CMesh* OneMesh = FBXMesh->m_MeshList[i];
		if (OneMesh->m_MaterialData == nullptr) { continue; }

		std::string ModelName = FileName + "_" + OneMesh->m_MaterialData->m_MaterialName;
		EATER_OPEN_WRITE_FILE(ModelName, "../Assets/Texture/Material/", ".Emat");
		auto m = GET_FORMAT_MATERIAL();

		m->Name = ModelName.c_str();
		m->Alpha = OneMesh->m_MaterialData->m_Alpha;

		if (OneMesh->m_MaterialData->m_DiffuseMap != nullptr)
		{
			m->DiffuseMap = OneMesh->m_MaterialData->m_DiffuseMap->m_BitMap.c_str();
		}
		else
		{
			m->DiffuseMap = "0";
		}

		if (OneMesh->m_MaterialData->m_NormalMap != nullptr)
		{
			m->NormalMap = OneMesh->m_MaterialData->m_NormalMap->m_BitMap.c_str();
		}
		else
		{
			m->NormalMap = "0";
		}

		if (OneMesh->m_MaterialData->m_EmissiveMap != nullptr)
		{
			m->EmissiveMap = OneMesh->m_MaterialData->m_EmissiveMap->m_BitMap.c_str();
		}
		else
		{
			m->EmissiveMap = "0";
		}

		if (OneMesh->m_MaterialData->m_ORMMap != nullptr)
		{
			m->ORMMap = OneMesh->m_MaterialData->m_ORMMap->m_BitMap.c_str();
		}
		else
		{
			m->ORMMap = "0";
		}
		

		m->SetMaterial(0, 0, 0);
		m->Tileing_X = 1;
		m->Tileing_Y = 1;
		m->SetColor(0, 0, 0);
		m->SetLimColor(0, 0, 0);
		m->LimFactor = 0;
		m->LimWidth = 0;
		SET_SAVE_MATERIAL();

		EATER_CLOSE_WRITE_FILE();

		std::string LoadName = "../Assets/Texture/Material/" + ModelName + ".Emat";
		EditorToolScene::MeshLoad(LoadName);
	}
}

void E_MaterialManager::Create(InstanceMaterial* m)
{
	EATER_OPEN_WRITE_FILE(m->Name, "../Assets/Texture/Material/", ".Emat");
	auto mMaterial			=  GET_FORMAT_MATERIAL();
	mMaterial->Name			= m->Name.c_str();
	mMaterial->Alpha		= m->Alpha;

	mMaterial->DiffuseMap	= m->DiffuseMap.c_str();
	mMaterial->EmissiveMap	= m->EmissiveMap.c_str();
	mMaterial->ORMMap		= m->ORMMap.c_str();
	mMaterial->NormalMap	= m->NormalMap.c_str();

	mMaterial->LimFactor	= m->LimFactor;
	mMaterial->LimWidth		= m->LimWidth;
	mMaterial->SkyLightIndex = 0;
	mMaterial->SetColor(m->AddColorR, m->AddColorG, m->AddColorB);
	mMaterial->SetLimColor(m->LimColorR, m->LimColorG, m->LimColorB);
	mMaterial->SetMaterial(m->Emissive, m->Roughness, m->Metallic);

	EATER_CLOSE_WRITE_FILE();
}

void E_MaterialManager::CreateBase(std::string Filename)
{
	Filename += "_Material";
	EATER_OPEN_WRITE_FILE(Filename, "../Assets/Texture/Material/", ".Emat");
	FM_MATERIAL* m = GET_FORMAT_MATERIAL();
	m->Name = "Base";
	SET_SAVE_MATERIAL();
	EATER_CLOSE_WRITE_FILE();
}

std::string E_MaterialManager::CutStr(std::string& Path)
{
	size_t start = Path.rfind("/") + 1;
	size_t end = Path.length() - start;
	std::string FileType = Path.substr(start, end);
	return FileType;
}

void E_MaterialManager::Initialize()
{
	
}

void E_MaterialManager::Release()
{


}

void E_MaterialManager::CreateEaterFile(GameObject* Obj)
{
	MeshFilter* mMeshFilter = Obj->GetComponent<MeshFilter>();
	EATER_OPEN_WRITE_FILE(FileName, FilePath, FileType);
	FM_MATERIAL* mMaterial = GET_FORMAT_MATERIAL();
	
	Material*			m	= mMeshFilter->GetMaterial();
	MaterialProperty*	mp	= m->m_MaterialData->Material_Property;

	mMaterial->Name			= m->m_MaterialData->Name;
	mMaterial->Alpha		= mp->Alpha;
	mMaterial->DiffuseMap	= m->GetDiffuseName();
	mMaterial->NormalMap	= m->GetNormalName();
	mMaterial->EmissiveMap	= m->GetEmissiveName();
	mMaterial->ORMMap		= m->GetORMName();
	mMaterial->LimFactor	= mp->LimLightFactor;
	mMaterial->LimWidth		= mp->LimLightWidth;
	mMaterial->SkyLightIndex = 0;
	
	mMaterial->SetColor(mp->AddColor.x, mp->AddColor.y, mp->AddColor.z);
	mMaterial->SetLimColor(mp->LimLightColor.x, mp->LimLightColor.y, mp->LimLightColor.z);
	mMaterial->SetMaterial(mp->EmissiveFactor, mp->RoughnessFactor, mp->MetallicFactor);
	SET_SAVE_MATERIAL();
	EATER_CLOSE_WRITE_FILE();
}

void E_MaterialManager::ChangeEaterFile(GameObject* Obj)
{


}

void E_MaterialManager::LoadData()
{


}
