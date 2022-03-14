#pragma once
#include <string>
#include <vector>
#include <map>

enum class  YAML_DATATYPE
{
	YAML_NULL,			//�����Ͱ� �������
	YAML_DATA_INT,		//int �� ������
	YAML_DATA_FLOAT,	//float�� ������
	YAML_DATA_STRING,	//string�� ������
};

enum class YAML_NODETYPE
{
	YAML_SCALAR,		//���ڰ� �����͸� �������ִ� ���
	YAML_MAP,			//Map�����͸� ������ �ִ� ���
	YAML_ARRAY,			//array�����͸� ������ �ִ� ���
	YAML_TOPNODE,		//�ƹ��͵� ���� �ֻ��� ���
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




