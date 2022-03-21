#include "EaterParser.h"
#include "EaterSaveData.h"
#include "EaterNode.h"
#include "EaterList.h"

#include <stdio.h>

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
			NodeList.push_back(mNode);
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
	WriteFile = fopen(Name.c_str(), "w");
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
		SetData = Data + LINE_TYPE;
	}
	else
	{
		SetData = Data + SP_TYPE;
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
	return (int)NodeList.size();
}

std::string& EaterParser::GetNodeName(int NodeCount)
{
	return NodeList[NodeCount]->Name;
}

std::string& EaterParser::GetMapData(int NodeCount, std::string& Key)
{
	return NodeList[NodeCount]->MapData[Key];
}

int EaterParser::ChoiceListData(int NodeCount, std::string& ListName)
{
	GetListData = &(NodeList[NodeCount]->ListData[ListName]);
	return (int)GetListData->F_Data.size();
}

void EaterParser::GetList(std::vector<float>* Data, int index)
{
	int Size = (int)GetListData->F_Data[index].size();
	Data->clear();
	Data->reserve(Size);
	for (int i = 0; i < Size; i++)
	{
		float OutData = stof(GetListData->F_Data[index][i]);
		Data->push_back(OutData);
	}
}

void EaterParser::GetList(std::vector<std::string>* Data, int index)
{
	int Size = (int)GetListData->F_Data[index].size();
	Data->clear();
	Data->reserve(Size);
	for (int i = 0; i < Size; i++)
	{
		std::string OutData = GetListData->F_Data[index][i];
		
		OutData = OutData.substr(0, OutData.rfind("\n") );
		Data->push_back(OutData);
	}
}

void EaterParser::ChangeMap(int NodeCount, std::string& Key, std::string& ChangeValue)
{


}

void EaterParser::ClearNode()
{
	int size = (int)NodeList.size();
	for (int i = 0; i < size; i++)
	{
		delete NodeList[i];
		NodeList[i] = nullptr;
	}
	NodeList.clear();
}




