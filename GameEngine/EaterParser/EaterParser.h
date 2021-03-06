#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "EaterSaveData.h"

class FM_VERTEX;
class FM_MATERIAL;
class FM_ANIMATION;
class FM_INDEX;
class FM_PARTICLE;



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
	void SetMaterial(const EATER_MATERIAL_DATA& Data);
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
	void SetVertex(EATER_VERTEX_POS& Base);
	void SetIndexStart(int IndexCount);
	void SetIndex(int& x,int& y,int& z);

	void SaveMaterial();
	void SaveParticle();
public:
	int GetNodeCount();
	std::string& GetNodeName(int NodeCount);
	std::string& GetMapData(int NodeCount,std::string& Key);
	int ChoiceListData(int NodeCount, std::string& ListName);

	void GetList(std::vector<float>* Data, int index);
	void GetList(std::vector<std::string>* Data, int index);
public:
	//맵을 변경한다
	void ChangeMap(int NodeCount,std::string& Key,std::string& ChangeValue);
	void ChangeList(int NodeCount, std::string& ListName, int cx_index, int cy_index, std::string& ChangeData);
	//변경된 값으로 저장한다
	void ChangeDataSave(std::string& FileName, std::string& OutPath, std::string& FileType);
public:
	FM_INDEX*		mIndex_Data;
	FM_MATERIAL*	mMaterial_Data;
	FM_VERTEX*		mVertex_Data;
	FM_ANIMATION*	mAnimation_Data;
	FM_PARTICLE*	mParticle_Data;
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

	FILE* WriteFile;
	FILE* ReadFile;

	std::vector<EaterNode*> mNodeList;
	EaterNode* mNode;
	EaterList* GetListData;
};

