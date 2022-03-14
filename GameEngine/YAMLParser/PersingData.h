#pragma once
#include <string>
#include <vector>
#include <map>

enum class  YAML_DATATYPE
{
	YAML_NULL,			//데이터가 비어있음
	YAML_DATA_INT,		//int 형 데이터
	YAML_DATA_FLOAT,	//float형 데이터
	YAML_DATA_STRING,	//string형 데이터
};

enum class YAML_NODETYPE
{
	YAML_SCALAR,		//숫자가 데이터를 가지고있는 노드
	YAML_MAP,			//Map데이터를 가지고 있는 노드
	YAML_ARRAY,			//array데이터를 가지고 있는 노드
	YAML_TOPNODE,		//아무것도 없는 최상위 노드
	YAML_NULL
};

class MapData
{
public:
	MapData()	= default;
	~MapData()	= default;

	std::string key;
	std::string Value;
};


class TopNode  
{
public:
	TopNode() = default;
	~TopNode() = default;

	int NodeListCount = 0;
	MapData* NodeList;
};




