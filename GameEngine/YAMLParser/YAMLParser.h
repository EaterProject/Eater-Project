#pragma once
#include <string>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include <unordered_map>

class YAMLNode;
class YAMLData;
class TopNode;
class YAMLParser
{
public:
	YAMLParser();
	~YAMLParser();
public:
	//파일 쓰기
	void CreateFile(std::string& Path,std::string& FileName);
	void CreateDocument(std::string Name);

	//맵 데이터 쓰기
	void Write_MapBegin(bool isFlow = false);
	void Write_MapEnd();
	void Write_SetMapData(std::string m_Key, std::string m_Value);
	void Write_SetMapData(std::string m_Key);
	//리스트 데이터 쓰기
	void Write_ListBegin();
	void Write_ListEnd();
	void Write_SetListData(std::string m_Data);
	void SaveFile();
public:
	//파일 데이터 읽기
	int OpenFile(std::string FilePath, std::vector<std::string>* OutPutData);	 //경로를 입력받아 노드를생성 
	//데이터를 어떤 타입으로 찾을것인지 셋팅한다
	void Find_String_Setting(std::vector<std::string>* keyList, std::vector<std::string>* DataList);
	void Find_float_Setting(std::vector<std::string>* keyList, std::vector<float>* DataList);
	void Find_int_Setting(std::vector<std::string>* keyList, std::vector<int>* DataList);
private:
	void FindNode();
	YAML::Node FindNode(int MaxCount,int NowCount,YAML::Node _Node);

	//데이터 읽기
	void Load_DataType(const YAML::Node mNode);
	void Load_map(YAML::const_iterator& it);
	void Load_Scalar(YAML::const_iterator& it);
	void Load_Sequence(YAML::const_iterator& it);

	void Load_map(YAML::Node _Node);
	void Load_Scalar(YAML::Node _Node);
	void Load_Sequence(YAML::Node _Node);
private:
	//초기값 생성
	std::string PushNode(YAML::Node* _KeyNode, YAML::Node _ValueNode);
private:
	std::vector<YAML::Node> mNodeList;							//모든 노드 리스트
	std::unordered_map<std::string, YAML::Node> ChildNodeList;	//노드의 개수
	std::unordered_map<std::string, int> ChildNodeCount;		//겹치는 자식 이름 개수
	std::vector<std::string>* KeyData;							//찾을 노드의 최상위 이름
private:
	//읽을 데이터를 모아두는 리스트
	std::vector<std::string>*	OutPutData_String;
	std::vector<float>*			OutPutData_Float;
	std::vector<int>*			OutPutData_Int;
	int DataType;
private:
	//쓰기 데이터를 모아두는 리스트
	YAML::Emitter yaml;
	//현재 어떤데이터를 쓰고있는지 체크한다
	bool is_Write_List;
	bool is_Write_Scalar;
	bool is_Write_Map;
private:
	//타입
	const int STRING_TYPE	= 0;
	const int INT_TYPE		= 1;
	const int FLOAT_TYPE	= 2;
private:
	std::ofstream WriteFile;
};

