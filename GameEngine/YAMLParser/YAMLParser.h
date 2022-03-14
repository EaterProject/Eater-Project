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
	//���� ����
	void CreateFile(std::string& Path,std::string& FileName);
	void CreateDocument(std::string Name);

	//�� ������ ����
	void Write_MapBegin(bool isFlow = false);
	void Write_MapEnd();
	void Write_SetMapData(std::string m_Key, std::string m_Value);
	void Write_SetMapData(std::string m_Key);
	//����Ʈ ������ ����
	void Write_ListBegin();
	void Write_ListEnd();
	void Write_SetListData(std::string m_Data);
	void SaveFile();
public:
	//���� ������ �б�
	int OpenFile(std::string FilePath, std::vector<std::string>* OutPutData);	 //��θ� �Է¹޾� ��带���� 
	//�����͸� � Ÿ������ ã�������� �����Ѵ�
	void Find_String_Setting(std::vector<std::string>* keyList, std::vector<std::string>* DataList);
	void Find_float_Setting(std::vector<std::string>* keyList, std::vector<float>* DataList);
	void Find_int_Setting(std::vector<std::string>* keyList, std::vector<int>* DataList);
private:
	void FindNode();
	YAML::Node FindNode(int MaxCount,int NowCount,YAML::Node _Node);

	//������ �б�
	void Load_DataType(const YAML::Node mNode);
	void Load_map(YAML::const_iterator& it);
	void Load_Scalar(YAML::const_iterator& it);
	void Load_Sequence(YAML::const_iterator& it);

	void Load_map(YAML::Node _Node);
	void Load_Scalar(YAML::Node _Node);
	void Load_Sequence(YAML::Node _Node);
private:
	//�ʱⰪ ����
	std::string PushNode(YAML::Node* _KeyNode, YAML::Node _ValueNode);
private:
	std::vector<YAML::Node> mNodeList;							//��� ��� ����Ʈ
	std::unordered_map<std::string, YAML::Node> ChildNodeList;	//����� ����
	std::unordered_map<std::string, int> ChildNodeCount;		//��ġ�� �ڽ� �̸� ����
	std::vector<std::string>* KeyData;							//ã�� ����� �ֻ��� �̸�
private:
	//���� �����͸� ��Ƶδ� ����Ʈ
	std::vector<std::string>*	OutPutData_String;
	std::vector<float>*			OutPutData_Float;
	std::vector<int>*			OutPutData_Int;
	int DataType;
private:
	//���� �����͸� ��Ƶδ� ����Ʈ
	YAML::Emitter yaml;
	//���� ������͸� �����ִ��� üũ�Ѵ�
	bool is_Write_List;
	bool is_Write_Scalar;
	bool is_Write_Map;
private:
	//Ÿ��
	const int STRING_TYPE	= 0;
	const int INT_TYPE		= 1;
	const int FLOAT_TYPE	= 2;
private:
	std::ofstream WriteFile;
};

