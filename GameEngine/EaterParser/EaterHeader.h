#pragma once
#include "EaterDLL.h"
#include "EaterSaveData.h"
#include <vector>

enum class VERTEX_TYPE
{
	BASE = 0, //(Pos,UV,Nomal,Tangent)
	SKIN = 1, //(Pos,UV,Nomal,Tangent,BoneIndex,BoneWeights)
};

///������ ����,�ݱ�,������ ����ϴ� �Լ���
EATER_PARSER_DLL void EATER_OPEN_FILE(std::string Path);
EATER_PARSER_DLL void EATER_CREATE_FILE(std::string FileName, std::string OutPath, std::string FileType = ".Eater");
EATER_PARSER_DLL void EATER_CLOSE_FILE();

///�ֻ����� �����ϱ����� �Լ���
EATER_PARSER_DLL void EATER_SET_NODE(std::string NodeName);

///�� ������ �б����� �Լ���
EATER_PARSER_DLL void EATER_SET_MAP(std::string key,std::string value);

///����Ʈ ������ �б����� �Լ���
EATER_PARSER_DLL void EATER_SET_LIST_START(std::string Name, int LineCount, int WordCount);
EATER_PARSER_DLL void EATER_SET_LIST(std::string Data,bool Line = false);
EATER_PARSER_DLL void EATER_SET_LIST(int Data,bool Line = false);
EATER_PARSER_DLL void EATER_SET_LIST(float Data,bool Line = false);
EATER_PARSER_DLL void EATER_SET_LIST_END();

///���ؽ� ������ �ֱ����� �Լ���
EATER_PARSER_DLL void EATER_SET_VERTEX_START(int VetexCount, VERTEX_TYPE Type);
EATER_PARSER_DLL void EATER_SET_VERTEX(EATER_VERTEX_BASE& Base);
EATER_PARSER_DLL void EATER_SET_VERTEX(EATER_VERTEX_SKIN& Skin);

///�ε��� ������ �ֱ����� �Լ���
EATER_PARSER_DLL void EATER_SET_INDEX_START(int IndexCount);
EATER_PARSER_DLL void EATER_SET_INDEX(int& x,int& y,int& z);

///��� ������ �������� �Լ���
EATER_PARSER_DLL int EATER_GET_NODE_COUNT();
EATER_PARSER_DLL std::string& EATER_GET_NODE_NAME(int NodeIndex);
EATER_PARSER_DLL std::string& EATER_GET_MAP(int NodeIndex,std::string Key);
EATER_PARSER_DLL int EATER_GET_LIST_CHOICE(int NodeCount, std::string ListName);

EATER_PARSER_DLL void EATER_GET_LIST(std::vector<float>* Data, int index);
EATER_PARSER_DLL void EATER_GET_LIST(std::vector<std::string>* Data, int index);

///��� ���� Ŭ����
EATER_PARSER_DLL void EATER_CLEAR_NODE();