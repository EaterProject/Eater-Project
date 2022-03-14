#include "YAMLHeader.h"
#include "YAMLParser.h"
#include <vector>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

YAMLParser* YAMLLoadManager = new YAMLParser();
int YAML_OPEN_FILE(std::string FilePath, std::vector<std::string>* OutPutData)
{
	return YAMLLoadManager->OpenFile(FilePath, OutPutData);
}

void YAML_FIND_STRING_DATA(std::vector<std::string>* DataList, int Count, std::string str, ...)
{
	std::vector<std::string> key;
	char* pszTemp = NULL;
	int ilen = 0;
	va_list arglist;
	va_start(arglist, str);
	key.push_back(str);
	
	for (int i = 0; i < Count - 1; i++)
	{
		key.push_back(va_arg(arglist, char*));
	}
	va_end(arglist);

	YAMLLoadManager->Find_String_Setting(&key, DataList);
}

void YAML_FIND_FLOAT_DATA(std::vector<float>* DataList, int Count, std::string str, ...)
{
	std::vector<std::string> key;
	char* pszTemp = NULL;
	int ilen = 0;
	va_list arglist;
	va_start(arglist, str);
	key.push_back(str);

	for (int i = 0; i < Count - 1; i++)
	{
		key.push_back(va_arg(arglist, char*));
	}
	va_end(arglist);

	YAMLLoadManager->Find_float_Setting(&key, DataList);
}

void YAML_FIND_INT_DATA(std::vector<int>* DataList, int Count, std::string str, ...)
{
	std::vector<std::string> key;
	char* pszTemp = NULL;
	int ilen = 0;
	va_list arglist;
	va_start(arglist, str);
	key.push_back(str);

	for (int i = 0; i < Count - 1; i++)
	{
		key.push_back(va_arg(arglist, char*));
	}
	va_end(arglist);

	YAMLLoadManager->Find_int_Setting(&key, DataList);
}

void YAML_CREATE_FILE(std::string FilePath, std::string FileName)
{
	YAMLLoadManager->CreateFile(FilePath, FileName);
}

void YAML_SAVE_FILE()
{
	YAMLLoadManager->SaveFile();
}

void YAML_WRITE_MAP(std::string _Key, std::string _Value)
{
	if (_Value == "NULL")
	{
		YAMLLoadManager->Write_SetMapData(_Key);
	}
	else
	{
		YAMLLoadManager->Write_SetMapData(_Key, _Value);
	}
}

 void YAML_WRITE_MAP_Begin(bool isFlow)
{
	 YAMLLoadManager->Write_MapBegin(isFlow);
}

 void YAML_WRITE_MAP_End()
{
	 YAMLLoadManager->Write_MapEnd();
}

  void YAML_WRITE_List(std::string _Key, std::string _Value)
 {
	 return  void();
 }

 void YAML_WRITE_List_Begin(bool isFlow)
 {
	 return  void();
 }

 void YAML_WRITE_List_End()
 {
	 return  void();
 }






