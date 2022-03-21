#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

struct EATER_VERTEX_BASE;
struct EATER_VERTEX_SKIN;
class EaterNode;
class EaterList;
class EaterParser
{
public:
	EaterParser();
	~EaterParser();

	void OPEN_FILE(std::string& Path);
	void CREATE_FILE(std::string& FileName,std::string& OutPath,std::string& FileType);
	void CLOSE_WRITE_FILE();
	void CLOSE_READ_FILE();
public:
	void SetNode(std::string& name);
	void SetMap(std::string& Key, std::string& value);
	void SetListStart(std::string& name, int LineCount, int WordCount);
	void SetList(std::string& Data,bool Line);
	void SetList(int Data,bool Line);
	void SetList(float Data,bool Line);
	void SetListEnd();
	void SetVertexStart(int VertexCount,int Type);
	void SetVertex(EATER_VERTEX_BASE& Base);
	void SetVertex(EATER_VERTEX_SKIN& Base);
	void SetIndexStart(int IndexCount);
	void SetIndex(int& x,int& y,int& z);
public:
	int GetNodeCount();
	std::string& GetNodeName(int NodeCount);
	std::string& GetMapData(int NodeCount,std::string& Key);
	int ChoiceListData(int NodeCount, std::string& ListName);

	void GetList(std::vector<float>* Data, int index);
	void GetList(std::vector<std::string>* Data, int index);
public:
	void ChangeMap(int NodeCount,std::string& Key,std::string& ChangeValue);

public:
	void ClearNode();
private:
	const char LINE_TYPE	= '\n';
	const char TAP_TYPE		= '\t';
	const char NODE_TYPE	= '#';
	const char MAP_TYPE		= '@';
	const char LIST_TYPE	= '$';
	const char END_TYPE		= '&';
	const char SP_TYPE		= ' ';

private:
	bool ListStart = false;
	int MaxWordCount = 0;
	int MaxLineCount = 0;
	int WordCount = 0;
	int LineCount = 0;
	std::string NowKey;

	//std::ifstream ReadFile;
	//std::ofstream WriteFile;

	FILE* WriteFile;
	FILE* ReadFile;

	std::vector<EaterNode*> NodeList;
	EaterNode* mNode;
	EaterList* GetListData;
};

