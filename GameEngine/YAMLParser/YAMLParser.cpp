#include "YAMLParser.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <cmath>
#include "YAMLParser.h"
#include "PersingData.h"
#pragma warning(disable : 4251)

#ifdef _DEBUG
#pragma comment(lib,"yaml-cppd")

#else
#pragma comment(lib,"yaml-cpp.lib")
#endif

YAMLParser::YAMLParser()
{
	is_Write_List	= false;
	is_Write_Scalar = false;
	is_Write_Map	= false;

	OutPutData_String	= nullptr;
	OutPutData_Float	= nullptr;
	OutPutData_Int		= nullptr;

	KeyData = nullptr;

	DataType = -1;
}

YAMLParser::~YAMLParser()
{


}

void YAMLParser::CreateFile(std::string& Path, std::string& FileName)
{
	int EndIndex = (int)Path.length() -1;
	if (Path[EndIndex] != '/')
	{
		Path += '/';
	}
	
	EndIndex = (int)FileName.length() - 1;
	if (FileName[EndIndex] != '.')
	{
		FileName += ".Eater";
	}

	std::string File;
	File += Path;
	File += FileName;

	//파일을 쓰기목적으로 연다
	WriteFile.open(File);

	Write_MapBegin();
	Write_SetMapData("오브젝트");

	Write_ListBegin();
	Write_SetListData("Rigidbody");
	Write_SetListData("Transform");
	Write_SetListData("Collider");
	yaml << YAML::Key << "형선" << YAML::Value << "value";
	Write_ListEnd();



	Write_MapEnd();
	


}

void YAMLParser::CreateDocument(std::string Name)
{
	yaml << Name;
}

void YAMLParser::Write_MapBegin(bool isFlow)
{
	if (isFlow == true)
	{
		yaml<< YAML::Flow <<YAML::BeginMap;
	}
	else
	{
		yaml << YAML::BeginMap;
	}
}

void YAMLParser::Write_MapEnd()
{
	yaml << YAML::EndMap;
}

void YAMLParser::Write_SetMapData(std::string m_Key, std::string m_Value)
{
	yaml << YAML::Key << m_Key << YAML::Value << m_Value;
}

void YAMLParser::Write_SetMapData(std::string m_Key)
{
	yaml << YAML::Key << m_Key;
}

void YAMLParser::Write_ListBegin()
{
	yaml << YAML::BeginSeq;
}

void YAMLParser::Write_ListEnd()
{
	yaml << YAML::EndSeq;
}

void YAMLParser::Write_SetListData(std::string m_Key)
{
	yaml <<YAML::Key << m_Key;
}


void YAMLParser::SaveFile()
{
	yaml << YAML::EndDoc;
	WriteFile << yaml.c_str();
	WriteFile.close();
}

int YAMLParser::OpenFile(std::string FilePath,std::vector<std::string>* OutPutData)
{
	//경로를 넣고 라이브러리 내부에서 데이터를 읽는다
	mNodeList = YAML::LoadAllFromFile(FilePath);
	//CreateFile("../Resources/Data","HsTest");
	//최상위 노드들먼저 읽는다 
	for (int i = 0; i < (int)mNodeList.size(); i++)
	{
		YAML::Node _Node = mNodeList[i];
		YAML::const_iterator _Node_it = _Node.begin();

		YAML::Node Key		= _Node_it->first;
		YAML::Node Value	= _Node_it->second;

		//리스트에 넣기전에 같은 값이 있는부터 체크후 넣어준다
		std::string KeyName = PushNode(&Key,Value);

		//밖으로 내보내줄 데이터
		(*OutPutData).push_back(KeyName);
	}

	//현재 노드의 사이즈를 내보내준다
	return (int)OutPutData->size();
}

void YAMLParser::FindNode()
{
	//찾고싶은 데이터의 최상위 노드부터 가져온다
	std::string Key = (*KeyData)[0];

	//자식 노드 리스트 중에서 노드를 가져온다
	YAML::Node _Node = (ChildNodeList[Key]);

	//재귀를 타면서 필요한 노드를 가져온다
	int MaxCount = (int)KeyData->size();
	YAML::Node _OutDataNode;
	if (MaxCount != 1)
	{
		_OutDataNode = FindNode(MaxCount,0,_Node);
	}
	else
	{
		_OutDataNode = _Node;
	}

	//읽고자 하는 노드의 타입을 선별한다
	switch (_OutDataNode.Type())
	{
	case YAML::NodeType::Null:
		break;
	case YAML::NodeType::Map:		//key, value값
		Load_map(_OutDataNode);
		break;
	case YAML::NodeType::Scalar:	//숫자 또는 문자
		Load_Scalar(_OutDataNode);
		break;
	case YAML::NodeType::Sequence:	//배열 , 리스트
		Load_Sequence(_OutDataNode);
		break;
	case YAML::NodeType::Undefined:
		break;
	}

	OutPutData_String = nullptr;
}

YAML::Node YAMLParser::FindNode(int MaxCount, int NowCount, YAML::Node _Node)
{
	NowCount++;
	std::string Name = (*KeyData)[NowCount];
	YAML::Node temp = _Node[Name];
	
	if (NowCount != MaxCount -1)
	{
		return FindNode(MaxCount, NowCount, temp);
	}
	
	return temp;
}

void YAMLParser::Find_String_Setting(std::vector<std::string>* keyList, std::vector<std::string>* DataList)
{
	KeyData = keyList;
	OutPutData_String = DataList;
	DataType = STRING_TYPE;

	FindNode();

	OutPutData_String = nullptr;
}

void YAMLParser::Find_float_Setting(std::vector<std::string>* keyList, std::vector<float>* DataList)
{
	KeyData = keyList;
	OutPutData_Float = DataList;
	DataType = FLOAT_TYPE;

	FindNode();

	OutPutData_Float = nullptr;
}

void YAMLParser::Find_int_Setting(std::vector<std::string>* keyList, std::vector<int>* DataList)
{
	KeyData = keyList;
	OutPutData_Int = DataList;
	DataType = INT_TYPE;

	FindNode();

	OutPutData_Int = nullptr;
}

std::string YAMLParser::PushNode(YAML::Node* _KeyNode,YAML::Node _ValueNode)
{	
	//노드를 재정렬 시킨다
	std::string KeyName	= (*_KeyNode).as<std::string>();
	KeyName += "_0";
	std::unordered_map<std::string, int>::iterator it = ChildNodeCount.find(KeyName);
	if (it == ChildNodeCount.end())
	{
		ChildNodeCount.insert({KeyName,0});
		ChildNodeList.insert({ KeyName,_ValueNode });
		return KeyName;
	}
	else
	{
		//겹치는 이름에 넘버를 넣어준다
		int Count = ChildNodeCount[KeyName];
		ChildNodeCount[KeyName]++;

		Count += 1;
		int End = KeyName.rfind("_");
		int Start = 0;

		std::string N = KeyName.substr(Start, End);
		N += "_" + std::to_string(Count);
		ChildNodeList.insert({ N,_ValueNode });
		return N;
	}
}

void YAMLParser::Load_DataType(const YAML::Node mBoxNode)
{
	//현재 노드를 읽는다
	YAML::const_iterator it = mBoxNode.begin();
	for (it; it != mBoxNode.end(); it++)
	{
		YAML::Node Temp = it->second;
		//현재 키값
		std::string Key = it->first.as<std::string>();
	
		//해당 타입에따라 데이터를 읽는다
		switch(Temp.Type())
		{
		case YAML::NodeType::Null:
			break;	
		case YAML::NodeType::Map:		//key, value값
			Load_map(it);
			break;
		case YAML::NodeType::Scalar:	//숫자또는 문자
			Load_Scalar(it);
			break;
		case YAML::NodeType::Sequence:	//배열 , 리스트
			Load_Sequence(it);
			break;
		case YAML::NodeType::Undefined:
			break;
		}
	}

}

void YAMLParser::Load_map(YAML::const_iterator& it)
{
	//한번더 반복문을 돌린다 결국엔 스칼라값으로 나올것...
	YAML::Node mNode = it->second;
	Load_DataType(mNode);
}

void YAMLParser::Load_map(YAML::Node _Node)
{
	YAML::const_iterator it = _Node.begin();
	for (it; it != _Node.end(); it++)
	{
		YAML::Node temp = it->second;
		
		if (temp.IsScalar() == true)
		{
			Load_Scalar(temp);
		}
		else if (temp.IsMap() == true)
		{
			Load_map(temp);
		}
		else if (temp.IsSequence() == true)
		{
			Load_Sequence(temp);
		}
	}
}

void YAMLParser::Load_Scalar(YAML::Node _Node)
{
	if (DataType == STRING_TYPE)
	{
		OutPutData_String->push_back(_Node.as<std::string>());
	}

	if (DataType == INT_TYPE)
	{
		OutPutData_Int->push_back(_Node.as<int>());
	}

	if (DataType == FLOAT_TYPE)
	{
		OutPutData_Float->push_back(_Node.as<float>());
	}
}

void YAMLParser::Load_Sequence(YAML::Node _Node)
{
	int Size = _Node.size();
	for (int i = 0; i < Size; i++)
	{
		YAML::Node temp = _Node[i];
		YAML::const_iterator Nowit = temp.begin();
		for (Nowit; Nowit != temp.end(); ++Nowit)
		{
			//배열안에 값이 무엇인가
			if (temp.IsScalar() == true) //숫자 혹은 문자
			{
				Load_Scalar(Nowit);
			}
			else if (temp.IsMap() == true) //key, value값
			{
				Load_map(Nowit);
			}
			else if (temp.IsSequence() == true)//배열 , 리스트
			{
				Load_Sequence(Nowit);
			}
		}
	}
}

void YAMLParser::Load_Scalar(YAML::const_iterator& it)
{
	OutPutData_String->push_back(it->second.as<std::string>());
}

void YAMLParser::Load_Sequence(YAML::const_iterator& it)
{
	std::string Value = it->first.as<std::string>();
	YAML::Node mNode= it->second;
	
	int Size = mNode.size();
	for (int i = 0; i < Size; i++)
	{
		YAML::Node temp = mNode[i];
		YAML::const_iterator Nowit = temp.begin();
		for (Nowit; Nowit != temp.end(); ++Nowit)
		{
			//배열안에 값이 무엇인가
			if (temp.IsScalar() == true) //숫자 혹은 문자
			{
				Load_Scalar(Nowit);
			}
			else if (temp.IsMap() == true) //key, value값
			{
				Load_map(Nowit);
			}
			else if (temp.IsSequence() == true)//배열 , 리스트
			{
				Load_Sequence(Nowit);
			}
		}
	}
}
