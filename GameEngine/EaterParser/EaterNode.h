#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "EaterSaveData.h"
#include "EaterList.h"
class EaterNode
{
public:
	EaterNode();
	~EaterNode();

	std::string Name;
	std::unordered_map<std::string, std::string> MapData;
	std::unordered_map<std::string, EaterList> ListData;


	std::vector<EATER_VERTEX_BASE> BaseVertex;
	std::vector<EATER_VERTEX_SKIN> SkinVertex;
	std::vector<int> Index;
private:
};

