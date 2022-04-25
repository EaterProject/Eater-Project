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

	EATER_OPEN_READ_FILE(Path);
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
			std::string DiffuseName		= CutStr(EATER_GET_MAP(i, "DiffuseMap"));
			std::string NormalName		= CutStr(EATER_GET_MAP(i, "NormalMap"));
			std::string EmissiveName	= CutStr(EATER_GET_MAP(i, "EmissiveMap"));
			std::string ORMName			= CutStr(EATER_GET_MAP(i, "ORMMap"));

			Data->Albedo = LoadManager::GetTexture(DiffuseName);
			Data->Normal = LoadManager::GetTexture(NormalName);
			Data->Emissive = LoadManager::GetTexture(EmissiveName);
			Data->ORM = LoadManager::GetTexture(ORMName);

			if (Data->Albedo) Data->Albedo->Name = DiffuseName;
			if (Data->Normal) Data->Normal->Name = NormalName;
			if (Data->Emissive) Data->Emissive->Name = EmissiveName;
			if (Data->ORM) Data->ORM->Name = ORMName;

			Data->Material_Property->EmissiveFactor = std::stof(EATER_GET_MAP(i, "Emissive"));
			Data->Material_Property->RoughnessFactor = std::stof(EATER_GET_MAP(i, "Roughness"));
			Data->Material_Property->MetallicFactor = std::stof(EATER_GET_MAP(i, "Metallic"));

			float Scale_X = std::stof(EATER_GET_MAP(i, "Tileing_X"));
			float Scale_Y = std::stof(EATER_GET_MAP(i, "Tileing_Y"));
			Data->Material_Property->TexTM = Matrix::CreateScale(1 / Scale_X, 1 / Scale_Y, 1.0f);

			Data->Material_Property->AddColor.x = std::stof(EATER_GET_MAP(i, "AddColor_R"));
			Data->Material_Property->AddColor.y = std::stof(EATER_GET_MAP(i, "AddColor_G"));
			Data->Material_Property->AddColor.z = std::stof(EATER_GET_MAP(i, "AddColor_B"));

			Data->Material_Property->LimLightColor.x = std::stof(EATER_GET_MAP(i, "LimColor_R"));
			Data->Material_Property->LimLightColor.y = std::stof(EATER_GET_MAP(i, "LimColor_G"));
			Data->Material_Property->LimLightColor.z = std::stof(EATER_GET_MAP(i, "LimColor_B"));

			Data->Material_Property->LimLightFactor = std::stof(EATER_GET_MAP(i, "LimFactor"));
			Data->Material_Property->LimLightWidth = std::stof(EATER_GET_MAP(i, "LimWidth"));

			Mat->m_MaterialData->Name = SaveName;

			// Material Buffer 府胶飘 火涝..
			LoadManager::MaterialList.insert({ SaveName, Mat });
		}
	}
	EATER_CLOSE_READ_FILE();
}
