#include "EaterManager.h"
#include "EaterHeader.h"

EaterManager::EaterManager()
{

}

EaterManager::~EaterManager()
{

}

void EaterManager::OpenFile(std::string& Path)
{
	//EATER_OPEN_FILE(Path);
	//
	//int Count = EATER_GET_NODE_COUNT();
	//for (int i = 0; i < Count; i++)
	//{
	//	std::string NodeName = EATER_GET_NODE_NAME(i);
	//
	//	if (NodeName == "STATIC")
	//	{
	//		std::string DiffuseMap	= EATER_GET_MAP(i, "DiffuseMap");
	//		std::string NormalMap	= EATER_GET_MAP(i, "NormalMap");
	//		std::string EmissiveMap = EATER_GET_MAP(i, "EmissiveMap");
	//		std::string ORMMap		= EATER_GET_MAP(i, "ORMMap");
	//	}
	//}
	//EATER_CLEAR_NODE();
}
