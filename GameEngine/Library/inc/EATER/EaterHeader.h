#pragma once
#include "FormatDLL.h"
#include "EaterSaveData.h"

#include "FM_MATERIAL.h"
#include "FM_VERTEX.h"
#include "FM_ANIMATION.h"
#include "FM_INDEX.h"
#include "FM_PARTICLE.h"


#include <vector>

enum class VERTEX_TYPE
{
	BASE = 0, //(Pos,UV,Nomal,Tangent)
	POS	 = 1, //(Pos)
	SKIN = 2, //(Pos,UV,Nomal,Tangent,BoneIndex,BoneWeights)
};

///������ ����,�ݱ�,������ ����ϴ� �Լ���
FORMAT_PARSER_DLL void EATER_OPEN_READ_FILE(std::string Path);
FORMAT_PARSER_DLL void EATER_OPEN_WRITE_FILE(std::string FileName, std::string OutPath, std::string FileType = ".Eater");
FORMAT_PARSER_DLL void EATER_CLOSE_WRITE_FILE(); ///���� ���ϴݱ�
FORMAT_PARSER_DLL void EATER_CLOSE_READ_FILE();	///���� ���ϴݱ�
FORMAT_PARSER_DLL void EATER_CLOSE_CHANGE_FILE(std::string FileName, std::string OutPath, std::string FileType = ".Eater"); ///���� ������ ���� �ݱ�

///�ֻ����� �����ϱ����� �Լ���
FORMAT_PARSER_DLL void EATER_SET_NODE(std::string NodeName);
FORMAT_PARSER_DLL int EATER_GET_NODE_COUNT();
FORMAT_PARSER_DLL std::string& EATER_GET_NODE_NAME(int NodeIndex);


///�� ������ �б�,����,������ ���� �Լ���
FORMAT_PARSER_DLL void EATER_SET_MAP(std::string key,std::string value);
FORMAT_PARSER_DLL void EATER_CHANGE_MAP(int NodeIndex, std::string key,std::string value);
FORMAT_PARSER_DLL std::string& EATER_GET_MAP(int NodeIndex,std::string Key);

///����Ʈ ������ �б�,����,������ ���� �Լ���
FORMAT_PARSER_DLL void EATER_SET_MATERIAL(const EATER_MATERIAL_DATA& Data);



FORMAT_PARSER_DLL void EATER_SET_LIST_START(std::string Name, int LineCount, int WordCount);
FORMAT_PARSER_DLL void EATER_SET_LIST(std::string Data,bool Line = false);
FORMAT_PARSER_DLL void EATER_SET_LIST(int Data,bool Line = false);
FORMAT_PARSER_DLL void EATER_SET_LIST(float Data,bool Line = false);
FORMAT_PARSER_DLL int  EATER_GET_LIST_CHOICE(int NodeCount, std::string ListName);
FORMAT_PARSER_DLL void EATER_GET_LIST(std::vector<float>* Data, int index);
FORMAT_PARSER_DLL void EATER_GET_LIST(std::vector<std::string>* Data, int index);
FORMAT_PARSER_DLL void EATER_CHANGE_LIST(int NodeCount, std::string ListName, int cx_index, int cy_index, std::string ChangeData);

///�ε���,���ؽ� ������ �ֱ����� �Լ���
FORMAT_PARSER_DLL void EATER_SET_VERTEX_START(int VetexCount, VERTEX_TYPE Type);
FORMAT_PARSER_DLL void EATER_SET_VERTEX(EATER_VERTEX_BASE& Base);
FORMAT_PARSER_DLL void EATER_SET_VERTEX(EATER_VERTEX_SKIN& Skin);
FORMAT_PARSER_DLL void EATER_SET_VERTEX(EATER_VERTEX_POS& Pos);
FORMAT_PARSER_DLL void EATER_SET_INDEX_START(int IndexCount);
FORMAT_PARSER_DLL void EATER_SET_INDEX(int& x,int& y,int& z);

FORMAT_PARSER_DLL void SET_SAVE_MATERIAL();
FORMAT_PARSER_DLL void SET_SAVE_PARTICLE();
FORMAT_PARSER_DLL FM_MATERIAL*	GET_FORMAT_MATERIAL();
FORMAT_PARSER_DLL FM_VERTEX*	GET_FORMAT_VERTEX(FM_VERTEX_TYPE Type);
FORMAT_PARSER_DLL FM_ANIMATION*	GET_FORMAT_ANIMATION();
FORMAT_PARSER_DLL FM_INDEX*		GET_FORMAT_INDEX();
FORMAT_PARSER_DLL FM_PARTICLE*	GET_FORMAT_PARTICLE();
