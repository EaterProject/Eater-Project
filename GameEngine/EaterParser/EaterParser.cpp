#include "EaterParser.h"
#include "EaterSaveData.h"
#include "EaterNode.h"
#include "EaterList.h"

#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

EaterParser::EaterParser()
{

}

EaterParser::~EaterParser()
{

}

void EaterParser::OPEN_FILE(std::string& Path)
{
	char Buffer[256];
	ReadFile = fopen(Path.c_str(), "r+");

	if (ReadFile == nullptr) { return; }
	while (feof(ReadFile) == 0)
	{
		//한줄을 읽어온다
		fgets(Buffer, 256, ReadFile);

		//첫번째 문자로 어떤타입인지 알아온다
		if (Buffer[0] == NODE_TYPE)
		{
			mNode = new EaterNode();
			mNode->Name = strtok(Buffer, "\n");
			mNode->Name.erase(0, 1);
			mNodeList.push_back(mNode);
		}
		else if (Buffer[0] == MAP_TYPE)
		{
			std::string Key		= strtok(Buffer, " ");
			std::string Value	= strtok(NULL, "\n");
			mNode->MapData.insert({ Key.erase(0,1),Value });
		}
		else if (Buffer[0] == LIST_TYPE)
		{
			std::string DataName		= strtok(Buffer, " ");
			DataName.erase(0, 1);

			std::string STR_LineCount	= strtok(NULL, " ");
			std::string STR_WordCount	= strtok(NULL, "\n");

			LineCount = 0;
			WordCount = 0;
			MaxWordCount = std::stoi(STR_WordCount);
			MaxLineCount = std::stoi(STR_LineCount);

			EaterList temp;
			mNode->ListData.insert({ DataName,temp });
			mNode->ListData[DataName].F_Data.resize(MaxLineCount);
			NowKey = DataName;
		}
		else if(Buffer[0] == END_TYPE)
		{
			break;
		}
		else
		{
			EaterList* List = &(mNode->ListData[NowKey]);
			for (int i = 0; i < MaxWordCount; i++)
			{
				List->F_Data[LineCount].reserve(MaxWordCount);
				if (i == 0)
				{
					char* Data = strtok(Buffer, " ");
					List->F_Data[LineCount].push_back(Data);
				}
				else if (i == MaxWordCount -1)
				{
					char* Data = strtok(NULL, "\n");
					List->F_Data[LineCount].push_back(Data);
					LineCount++;
				}
				else
				{
					char* Data = strtok(NULL, " ");
					List->F_Data[LineCount].push_back(Data);
				}
			}
		}
	}
}

void EaterParser::CREATE_FILE(std::string& FileName, std::string& OutPath, std::string& FileType)
{
	std::string Name = OutPath + FileName + FileType;
	WriteFile = fopen(Name.c_str(), "w+");
}

void EaterParser::CLOSE_WRITE_FILE()
{
	fputs("&",WriteFile);
	fclose(WriteFile);
}

void EaterParser::CLOSE_READ_FILE()
{
	ClearNode();
	fclose(ReadFile);
}

void EaterParser::SetMaterial(const EATER_MATERIAL_DATA& Data)
{
	std::string SetData;
	SetData += MAP_TYPE + Data.MaterialName + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Alpha) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.DiffuseMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.NormalMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.EmissiveMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.ORMMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Emissive) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Roughness) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Metallic) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Tileing_X) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.Tileing_Y) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.AddColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.AddColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.AddColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.AddColor_A) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.LimColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.LimColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.LimColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.LimFactor) + LINE_TYPE;
	SetData += MAP_TYPE + std::to_string(Data.LimWidth) + LINE_TYPE;

	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetNode(std::string& name)
{
	std::string SetData =  NODE_TYPE + name + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetMap(std::string& Key, std::string& value)
{
	std::string SetData = MAP_TYPE + Key + SP_TYPE + value + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetListStart(std::string& name,int LineCount ,int WordCount)
{
	std::string SetData = LIST_TYPE + name + SP_TYPE + std::to_string(LineCount) + SP_TYPE + std::to_string(WordCount) + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetList(std::string& Data,bool Line)
{
	std::string SetData;
	if (Line == true)
	{
		if (Data == "")
		{
			Data = "NO";
			SetData = Data + LINE_TYPE;
		}
		else
		{
			SetData = Data + LINE_TYPE;
		}
	}
	else
	{
		if (Data == "")
		{
			Data = "NO";
			SetData = Data + SP_TYPE;
		}
		else
		{
			SetData = Data + SP_TYPE;
		}
	}
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetList(int Data, bool Line)
{
	std::string SetData;
	if (Line == true)
	{
		SetData = std::to_string(Data) + LINE_TYPE;
	}
	else
	{
		SetData = std::to_string(Data) + SP_TYPE;
	}
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetList(float Data, bool Line)
{
	std::string SetData;
	if (Line == true)
	{
		SetData =  std::to_string(Data) + LINE_TYPE;
	}
	else
	{
		SetData = std::to_string(Data) + SP_TYPE;
	}
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetListEnd()
{

}

void EaterParser::SetVertexStart(int VertexCount, int Type)
{
	if (Type == 0)
	{
		std::string SetData;
		std::string Name = "Vertex";
		SetData =  LIST_TYPE + Name + SP_TYPE + std::to_string(VertexCount) + SP_TYPE + std::to_string(11) + LINE_TYPE;
		fputs(SetData.c_str(), WriteFile);
	}
	else if (Type == 1)
	{
		std::string SetData;
		std::string Name = "Vertex";
		SetData = LIST_TYPE + Name + SP_TYPE + std::to_string(VertexCount) + SP_TYPE + std::to_string(3) + LINE_TYPE;
		fputs(SetData.c_str(), WriteFile);
	}
	else
	{
		std::string SetData;
		std::string Name = "Vertex";
		SetData = LIST_TYPE + Name + SP_TYPE + std::to_string(VertexCount) + SP_TYPE + std::to_string(27) + LINE_TYPE;
		fputs(SetData.c_str(), WriteFile);
	}
}

void EaterParser::SetVertex(EATER_VERTEX_BASE& Base)
{
	std::string SetData;
	SetData  = std::to_string(Base.POS_X)		+SP_TYPE	+ std::to_string(Base.POS_Y)		+ SP_TYPE	+	std::to_string(Base.POS_Z)		+ SP_TYPE;
	SetData += std::to_string(Base.UV_X)		+SP_TYPE	+ std::to_string(Base.UV_Y)			+ SP_TYPE;
	SetData += std::to_string(Base.NOMAL_X)		+SP_TYPE	+ std::to_string(Base.NOMAL_Y)		+ SP_TYPE	+	std::to_string(Base.NOMAL_Z)	+ SP_TYPE;
	SetData += std::to_string(Base.TANGENT_X)	+SP_TYPE	+ std::to_string(Base.TANGENT_Y)	+ SP_TYPE	+	std::to_string(Base.TANGENT_Z)	+LINE_TYPE;
	fputs(SetData.c_str(),WriteFile);
}

void EaterParser::SetVertex(EATER_VERTEX_SKIN& Base)
{
	std::string SetData;
	SetData  = std::to_string(Base.POS_X)		+ SP_TYPE + std::to_string(Base.POS_Y)		+ SP_TYPE + std::to_string(Base.POS_Z)		+ SP_TYPE;
	SetData += std::to_string(Base.UV_X)		+ SP_TYPE + std::to_string(Base.UV_Y)		+ SP_TYPE;
	SetData += std::to_string(Base.NOMAL_X)		+ SP_TYPE + std::to_string(Base.NOMAL_Y)	+ SP_TYPE + std::to_string(Base.NOMAL_Z)	+ SP_TYPE;
	SetData += std::to_string(Base.TANGENT_X)	+ SP_TYPE + std::to_string(Base.TANGENT_Y)	+ SP_TYPE + std::to_string(Base.TANGENT_Z)	+ SP_TYPE;

	for (int i = 0; i < 8; i++)
	{
		SetData += std::to_string(Base.BONE_INDEX[i]) + SP_TYPE;
	}

	for (int i = 0; i < 8; i++)
	{
		if (i == 7)
		{
			SetData += std::to_string(Base.BONE_WEIGHTS[i]) + LINE_TYPE;
		}
		else
		{
			SetData += std::to_string(Base.BONE_WEIGHTS[i]) + SP_TYPE;
		}
	}

	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetVertex(EATER_VERTEX_POS& Base)
{
	std::string SetData;
	SetData = std::to_string(Base.POS_X) + SP_TYPE + std::to_string(Base.POS_Y) + SP_TYPE + std::to_string(Base.POS_Z) + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetIndexStart(int IndexCount)
{
	std::string SetData;
	std::string Name = "Index";
	SetData = LIST_TYPE + Name + SP_TYPE + std::to_string(IndexCount)+ SP_TYPE + std::to_string(3) + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SetIndex(int& x, int& y, int& z)
{
	std::string SetData;
	SetData = std::to_string(x) + SP_TYPE + std::to_string(y) + SP_TYPE + std::to_string(z) + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

int EaterParser::GetNodeCount()
{
	return (int)mNodeList.size();
}

std::string& EaterParser::GetNodeName(int NodeCount)
{
	return mNodeList[NodeCount]->Name;
}

std::string& EaterParser::GetMapData(int NodeCount, std::string& Key)
{
	return mNodeList[NodeCount]->MapData[Key];
}

int EaterParser::ChoiceListData(int NodeCount, std::string& ListName)
{
	if (mNodeList[NodeCount]->ListData.find(ListName) 
		== mNodeList[NodeCount]->ListData.end())
	{
		return -1;
	}

	GetListData = &(mNodeList[NodeCount]->ListData[ListName]);
	return (int)GetListData->F_Data.size();
}

void EaterParser::GetList(std::vector<float>* Data, int index)
{
	int Size = (int)GetListData->F_Data[index].size();
	Data->clear();
	Data->resize(Size);
	for (int i = 0; i < Size; i++)
	{
		(*Data)[i] = stof(GetListData->F_Data[index][i]);
	}
}

void EaterParser::GetList(std::vector<std::string>* Data, int index)
{
	int Size = (int)GetListData->F_Data[index].size();
	Data->clear();
	Data->resize(Size);
	for (int i = 0; i < Size; i++)
	{
		std::string OutData = GetListData->F_Data[index][i];
		(*Data)[i] = OutData.substr(0, OutData.rfind("\n"));
	}
}

void EaterParser::ChangeMap(int NodeCount, std::string& Key, std::string& ChangeValue)
{
	mNodeList[NodeCount]->MapData[Key] = ChangeValue;
}

void EaterParser::ChangeList(int NodeCount,std::string &ListName, int cx_index,int cy_index,std::string& ChangeData)
{
	mNodeList[NodeCount]->ListData[ListName].F_Data[cy_index][cx_index] = ChangeData;
}

void EaterParser::ChangeDataSave(std::string& FileName, std::string& OutPath, std::string& FileType)
{
	fclose(ReadFile);

	CREATE_FILE(FileName, OutPath, FileType);

	int NodeCount = mNodeList.size();
	for (int i = 0; i < NodeCount; i++)
	{
		std::string SetData = NODE_TYPE + mNodeList[i]->Name + LINE_TYPE;
		fputs(SetData.c_str(), WriteFile);

		//맵데이터를 순환후 변경한값으로 저장
		std::unordered_map<std::string, std::string>::iterator Start_it	= mNodeList[i]->MapData.begin();
		std::unordered_map<std::string, std::string>::iterator End_it	= mNodeList[i]->MapData.end();
		for (Start_it; Start_it != End_it; Start_it++)
		{
			SetData = MAP_TYPE + Start_it->first+ SP_TYPE + Start_it->second + LINE_TYPE;
			fputs(SetData.c_str(), WriteFile);
		}

		//리스트 데이터를 순환후 변경한값으로 저장
		std::unordered_map<std::string,EaterList>::iterator List_Start_it	= mNodeList[i]->ListData.begin();
		std::unordered_map<std::string,EaterList>::iterator List_End_it		= mNodeList[i]->ListData.end();
		for (List_Start_it; List_Start_it != List_End_it; List_Start_it++)
		{
			std::string ListName	= List_Start_it->first;
			EaterList& Data			= List_Start_it->second;

			//행 사이즈
			int VerticlaSize	= (int)Data.F_Data[0].size();
			//열 사이즈
			int HorizontalSize	= (int)Data.F_Data.size();

			//리스트의 시작부분을 나타내는 부분을 넣어줌 ex)Vectrex 32(열 개수) 11(행 개수)
			SetData = LIST_TYPE + ListName+SP_TYPE+std::to_string(HorizontalSize)+SP_TYPE+std::to_string(VerticlaSize)+LINE_TYPE;
		
			fputs(SetData.c_str(), WriteFile);

			//행,열 된 리스트를 쓰기
			for (int i = 0; i < HorizontalSize; i++)
			{
				for (int j = 0; j< VerticlaSize; j++)
				{	
					//마지막부분은 줄바꿈 아닌경우에는 띄어쓰기만
					if (j == VerticlaSize - 1)
					{
						SetData = Data.F_Data[i][j] + LINE_TYPE;
						fputs(SetData.c_str(), WriteFile);
					}
					else
					{
						SetData = Data.F_Data[i][j] + SP_TYPE;
						fputs(SetData.c_str(), WriteFile);
					}
				}
			}
		}
	}
	//마지막 끝부분을 넣어준다
	std::string Data;
	Data = END_TYPE;
	fputs(Data.c_str(), WriteFile);

	ClearNode();
	fclose(WriteFile);
}

void EaterParser::ClearNode()
{
	int size = (int)mNodeList.size();
	for (int i = 0; i < size; i++)
	{
		delete mNodeList[i];
		mNodeList[i] = nullptr;
	}
	mNodeList.clear();
}




