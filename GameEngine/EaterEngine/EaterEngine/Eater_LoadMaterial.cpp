#include "Eater_LoadMaterial.h"
#include "EaterHeader.h"
#include "Material.h"
#include "EngineData.h"
#include "LoadManager.h"
Eater_LoadMaterial::Eater_LoadMaterial()
{


}

Eater_LoadMaterial::~Eater_LoadMaterial()
{


}

void Eater_LoadMaterial::LoadData(std::string& Path)
{
	std::size_t Start = Path.rfind('/') + 1;
	std::size_t End = Path.rfind('.') - Start;

	std::string SaveName = Path.substr(Start, End);

	EATER_OPEN_FILE(Path);
	int Count = EATER_GET_NODE_COUNT();
	for (int i = 0; i < Count; i++)
	{
		// 货肺款 Material 积己..
		Material* Mat = new Material();
		Mat->Name = SaveName;

		MaterialBuffer* Data = Mat->m_MaterialData;
		std::string NodeName = EATER_GET_NODE_NAME(i);
		if (NodeName == "EATERMAT")
		{
			std::string DiffuseName		= CutStr(EATER_GET_MAP(i, "Diffuse"));
			std::string NormalName		= CutStr(EATER_GET_MAP(i, "Normal"));
			std::string EmissiveName	= CutStr(EATER_GET_MAP(i, "Emissive"));
			std::string ORMName			= CutStr(EATER_GET_MAP(i, "ORM"));

			Data->Albedo = LoadManager::GetTexture(DiffuseName);
			Data->Normal = LoadManager::GetTexture(NormalName);
			Data->Emissive = LoadManager::GetTexture(EmissiveName);
			Data->ORM = LoadManager::GetTexture(ORMName);

			if (Data->Albedo) Data->Albedo->Name = DiffuseName;
			if (Data->Normal) Data->Normal->Name = NormalName;
			if (Data->Emissive) Data->Emissive->Name = EmissiveName;
			if (Data->ORM) Data->ORM->Name = ORMName;

			Data->Material_SubData->RoughnessFactor = std::stof(EATER_GET_MAP(i, "Roughness"));
			Data->Material_SubData->MetallicFactor = std::stof(EATER_GET_MAP(i, "Metallic"));

			float Scale_X = std::stof(EATER_GET_MAP(i, "Tileing_X"));
			float Scale_Y = std::stof(EATER_GET_MAP(i, "Tileing_Y"));
			Data->Material_SubData->TexTM = Matrix::CreateScale(1 / Scale_X, 1 / Scale_Y, 1.0f);

			Data->Material_SubData->AddColor.x = std::stof(EATER_GET_MAP(i, "AddColor_R"));
			Data->Material_SubData->AddColor.y = std::stof(EATER_GET_MAP(i, "AddColor_G"));
			Data->Material_SubData->AddColor.z = std::stof(EATER_GET_MAP(i, "AddColor_B"));
			Data->Material_SubData->AddColor.w = std::stof(EATER_GET_MAP(i, "AddColor_A"));

			// Material Buffer 府胶飘 火涝..
			LoadManager::MaterialList.insert({ SaveName, Mat });
		}
	}
	EATER_CLEAR_NODE();
}
