#include "E_MaterialManager.h"
#include "ParserData.h"
#include "EaterHeader.h"
#include "EditorToolScene.h"

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

		std::string ModelName = SaveFileName + "_" + OneMesh->m_MaterialData->m_MaterialName;
		EATER_OPEN_WRITE_FILE(ModelName, "../Assets/Texture/Material/", ".Emat");

		EATER_SET_NODE("EATERMAT");
		
		EATER_SET_MAP("MaterialName", ModelName);
		if (OneMesh->m_MaterialData->m_Alpha == true)
		{
			EATER_SET_MAP("Alpha", "YES");
		}
		else
		{
			EATER_SET_MAP("Alpha", "NO");
		}

		if (OneMesh->m_MaterialData->m_DiffuseMap != nullptr)
		{
			EATER_SET_MAP("DiffuseMap", CutStr(OneMesh->m_MaterialData->m_DiffuseMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("DiffuseMap", "NO");
		}

		if (OneMesh->m_MaterialData->m_NormalMap != nullptr)
		{
			EATER_SET_MAP("NormalMap",  CutStr(OneMesh->m_MaterialData->m_NormalMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("NormalMap", "NO");
		}

		if (OneMesh->m_MaterialData->m_EmissiveMap != nullptr)
		{
			EATER_SET_MAP("EmissiveMap", CutStr(OneMesh->m_MaterialData->m_EmissiveMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("EmissiveMap", "NO");
		}

		if (OneMesh->m_MaterialData->m_ORMMap != nullptr)
		{
			EATER_SET_MAP("ORMMap", CutStr(OneMesh->m_MaterialData->m_ORMMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("ORMMap", "NO");
		}

		EATER_SET_MAP("Emissive", "0");
		EATER_SET_MAP("Roughness", "0");
		EATER_SET_MAP("Metallic", "0");

		EATER_SET_MAP("Tileing_X", "1");
		EATER_SET_MAP("Tileing_Y", "1");

		EATER_SET_MAP("AddColor_R", "0");
		EATER_SET_MAP("AddColor_G", "0");
		EATER_SET_MAP("AddColor_B", "0");
		EATER_SET_MAP("AddColor_A", "0");

		EATER_SET_MAP("LimColor_R", "0");
		EATER_SET_MAP("LimColor_G", "0");
		EATER_SET_MAP("LimColor_B", "0");

		EATER_SET_MAP("LimFactor", "0");
		EATER_SET_MAP("LimWidth", "0");

		EATER_CLOSE_WRITE_FILE();

		std::string LoadName = "../Assets/Texture/Material/" + ModelName + ".Emat";
		EditorToolScene::MeshLoad(LoadName);
	}
}

void E_MaterialManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}

void E_MaterialManager::Create(InstanceMaterial* m)
{
	EATER_OPEN_WRITE_FILE(m->Name, "../Assets/Texture/Material/", ".Emat");
	EATER_SET_NODE("EATERMAT");

	EATER_SET_MAP("MaterialName", m->Name);
	if (m->Alpha == true)
	{
		EATER_SET_MAP("Alpha", "YES");
	}
	else
	{
		EATER_SET_MAP("Alpha", "NO");
	}
	
	EATER_SET_MAP("DiffuseMap", m->DiffuseMap);
	EATER_SET_MAP("Nomal", m->NormalMap);
	EATER_SET_MAP("EmissiveMap", m->EmissiveMap);
	EATER_SET_MAP("ORMMap", m->ORMMap);

	EATER_SET_MAP("Emissive", std::to_string(m->Emissive));
	EATER_SET_MAP("Roughness", std::to_string(m->Roughness));
	EATER_SET_MAP("Metallic", std::to_string(m->Metallic));

	EATER_SET_MAP("Tileing_X", "1");
	EATER_SET_MAP("Tileing_Y", "1");

	EATER_SET_MAP("AddColor_R", std::to_string(m->AddColorR));
	EATER_SET_MAP("AddColor_G", std::to_string(m->AddColorG));
	EATER_SET_MAP("AddColor_B", std::to_string(m->AddColorB));
	EATER_SET_MAP("AddColor_A", std::to_string(1));

	//EATER_SET_MAP("LimColor_R", std::to_string(m->AddColorR));
	//EATER_SET_MAP("LimColor_G", std::to_string(m->AddColorR));
	//EATER_SET_MAP("LimColor_B", std::to_string(m->AddColorR));
	//
	//EATER_SET_MAP("LimFactor", "0");
	//EATER_SET_MAP("LimWidth", "0");
	EATER_CLOSE_WRITE_FILE();
}




std::string E_MaterialManager::CutStr(std::string& Path)
{
	size_t start = Path.rfind("/") + 1;
	size_t end = Path.length() - start;
	std::string FileType = Path.substr(start, end);
	return FileType;
}