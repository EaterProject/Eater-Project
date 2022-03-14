#pragma once
#include <vector>
#include <map>
#include <string>
#include "EaterSaveData.h"
#include "EaterList.h"
class EaterNode
{
public:
	EaterNode();
	~EaterNode();

	std::string Name;
	std::map<std::string, std::string> MapData;
	std::map<std::string, EaterList> ListData;


	std::vector<EATER_VERTEX_BASE> BaseVertex;
	std::vector<EATER_VERTEX_SKIN> SkinVertex;
	std::vector<int> Index;
private:
};

