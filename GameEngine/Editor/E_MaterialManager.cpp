#include "E_MaterialManager.h"
#include "ParserData.h"
#include "EaterHeader.h"

E_MaterialManager::E_MaterialManager()
{

}

E_MaterialManager::~E_MaterialManager()
{

}

void E_MaterialManager::ChangeEaterFile(ParserData::CModel* FBXMesh)
{
	int MeshSize = FBXMesh->m_MeshList.size();
	for (int i = 0; i < MeshSize; i++)
	{
		ParserData::CMesh* OneMesh = FBXMesh->m_MeshList[i];
		if (OneMesh->m_MaterialData == nullptr) { continue; }

		std::string ModelName = SaveFileName + "_" + OneMesh->m_MaterialData->m_MaterialName;
		EATER_CREATE_FILE(ModelName, "../Assets/Texture/Material/", ".Emat");

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
			EATER_SET_MAP("Diffuse", CutStr(OneMesh->m_MaterialData->m_DiffuseMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("Diffuse", "NO");
		}

		if (OneMesh->m_MaterialData->m_NormalMap != nullptr)
		{
			EATER_SET_MAP("Normal",  CutStr(OneMesh->m_MaterialData->m_NormalMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("Normal", "NO");
		}

		if (OneMesh->m_MaterialData->m_EmissiveMap != nullptr)
		{
			EATER_SET_MAP("Emissive", CutStr(OneMesh->m_MaterialData->m_EmissiveMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("Emissive", "NO");
		}

		if (OneMesh->m_MaterialData->m_ORMMap != nullptr)
		{
			EATER_SET_MAP("ORM", CutStr(OneMesh->m_MaterialData->m_ORMMap->m_BitMap));
		}
		else
		{
			EATER_SET_MAP("ORM", "NO");
		}

		EATER_SET_MAP("Roughness", "0");
		EATER_SET_MAP("Metallic", "0");

		EATER_SET_MAP("Tileing_X", "1");
		EATER_SET_MAP("Tileing_Y", "1");

		EATER_SET_MAP("BaseColor_R", "1");
		EATER_SET_MAP("BaseColor_G", "0.5");
		EATER_SET_MAP("BaseColor_B", "0.5");
		EATER_SET_MAP("BaseColor_A", "1");

		EATER_SET_MAP("AddColor_R", "1");
		EATER_SET_MAP("AddColor_G", "1");
		EATER_SET_MAP("AddColor_B", "1");
		EATER_SET_MAP("AddColor_A", "1");

		EATER_CLOSE_FILE();
	}
}

void E_MaterialManager::SetFileName(std::string& FileName)
{
	SaveFileName = FileName;
}

std::string E_MaterialManager::CutStr(std::string& Path)
{
	size_t start = Path.rfind("/") + 1;
	size_t end = Path.length() - start;
	std::string FileType = Path.substr(start, end);
	return FileType;
}