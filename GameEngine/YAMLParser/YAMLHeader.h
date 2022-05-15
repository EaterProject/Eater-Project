#pragma once
#include "YAMLDLL.h"
#include <string>
#include <vector>
#include <cstdarg>

/// <summary>
/// Yaml�� �� ������ ����
/// </summary>
/// <param name="FilePath">���� ���</param>
/// <param name="OutPutData">������ ������ �ʿ��� Key��(������ �̸�)</param>
/// <returns>�� ��� ����</returns>
extern "C" YAML_PARSER_DLL int YAML_OPEN_FILE(std::string FilePath, std::vector<std::string>*OutPutData);

/// <summary>
/// Key������ �ش� ����� ���� �д´� ,float , String ,int
/// </summary>
/// <param name="DataList">�����͸� ��ƿ� string Vector</param>
/// <param name="Count">�����μ� ����</param>
/// <param name="str">ù��° �����μ� Ÿ��</param>
/// <param name="">�����μ���</param>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_FIND_STRING_DATA(std::vector<std::string>*DataList, int Count,std::string str,...);
extern "C" YAML_PARSER_DLL void YAML_FIND_FLOAT_DATA(std::vector<float>*DataList, int Count,std::string str,...);
extern "C" YAML_PARSER_DLL void YAML_FIND_INT_DATA(std::vector<int>*DataList, int Count,std::string str,...);

/// <summary>
/// ������ ������ ����
/// </summary>
/// <param name="FilePath">������ ���</param>
/// <param name="FileName">������ �̸�</param>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_CREATE_FILE(std::string FilePath, std::string FileName);

/// <summary>
/// ���Ⱑ �ϼ��� ������ �����Ų��
/// </summary>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_SAVE_FILE();


extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP(std::string _Key, std::string _Value = "NULL");
extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP_Begin(bool isFlow = false);
extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP_End();


extern "C" YAML_PARSER_DLL void YAML_WRITE_List(std::string _Key, std::string _Value = "NULL");
extern "C" YAML_PARSER_DLL void YAML_WRITE_List_Begin(bool isFlow = false);
extern "C" YAML_PARSER_DLL void YAML_WRITE_List_End();






