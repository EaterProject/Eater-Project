#pragma once
#include "YAMLDLL.h"
#include <string>
#include <vector>
#include <cstdarg>

/// <summary>
/// Yaml로 된 파일을 연다
/// </summary>
/// <param name="FilePath">파일 경로</param>
/// <param name="OutPutData">파일을 읽을떄 필요한 Key값(노드들의 이름)</param>
/// <returns>총 노드 개수</returns>
extern "C" YAML_PARSER_DLL int YAML_OPEN_FILE(std::string FilePath, std::vector<std::string>*OutPutData);

/// <summary>
/// Key값으로 해당 노드의 값을 읽는다 ,float , String ,int
/// </summary>
/// <param name="DataList">데이터를 담아올 string Vector</param>
/// <param name="Count">가변인수 개수</param>
/// <param name="str">첫번째 가변인수 타입</param>
/// <param name="">가변인수들</param>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_FIND_STRING_DATA(std::vector<std::string>*DataList, int Count,std::string str,...);
extern "C" YAML_PARSER_DLL void YAML_FIND_FLOAT_DATA(std::vector<float>*DataList, int Count,std::string str,...);
extern "C" YAML_PARSER_DLL void YAML_FIND_INT_DATA(std::vector<int>*DataList, int Count,std::string str,...);

/// <summary>
/// 저장할 파일을 생성
/// </summary>
/// <param name="FilePath">파일의 경로</param>
/// <param name="FileName">파일의 이름</param>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_CREATE_FILE(std::string FilePath, std::string FileName);

/// <summary>
/// 쓰기가 완성된 파일을 저장시킨다
/// </summary>
/// <returns></returns>
extern "C" YAML_PARSER_DLL void YAML_SAVE_FILE();


extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP(std::string _Key, std::string _Value = "NULL");
extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP_Begin(bool isFlow = false);
extern "C" YAML_PARSER_DLL void YAML_WRITE_MAP_End();


extern "C" YAML_PARSER_DLL void YAML_WRITE_List(std::string _Key, std::string _Value = "NULL");
extern "C" YAML_PARSER_DLL void YAML_WRITE_List_Begin(bool isFlow = false);
extern "C" YAML_PARSER_DLL void YAML_WRITE_List_End();






