#include "EaterParser.h"
#include "EaterSaveData.h"
#include "EaterNode.h"
#include "EaterList.h"
#include <stdio.h>

#include "FM_ANIMATION.h"
#include "FM_INDEX.h"
#include "FM_VERTEX.h"
#include "FM_MATERIAL.h"
#include "FM_PARTICLE.h"

#define _CRT_SECURE_NO_WARNINGS

EaterParser::EaterParser()
{
	mIndex_Data			= new FM_INDEX();
	mMaterial_Data		= new FM_MATERIAL();
	mVertex_Data		= new FM_VERTEX();
	mAnimation_Data		= new FM_ANIMATION();
	mParticle_Data		= new FM_PARTICLE();
}

EaterParser::~EaterParser()
{
	delete mAnimation_Data;
	delete mMaterial_Data;
	delete mVertex_Data;
	delete mIndex_Data;
	delete mParticle_Data;
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

#include <windows.h>

void EaterParser::CREATE_FILE(std::string& FileName, std::string& OutPath, std::string& FileType)
{
	std::string Name = OutPath + FileName + FileType;

	// 시발
	do 
	{
		WriteFile = fopen(Name.c_str(), "w+");
	} 
	while (WriteFile == nullptr);
}

void EaterParser::CLOSE_WRITE_FILE()
{
	fputs("&",WriteFile);
	if (fclose(WriteFile) == -1)
	{
		HWND hWndActive = ::GetForegroundWindow();
		MessageBox(hWndActive, L"파일이 제대로 닫치지않음", L"Error", MB_OK);
	}
}

void EaterParser::CLOSE_READ_FILE()
{
	ClearNode();
	fclose(ReadFile);
}

void EaterParser::SetMaterial(const EATER_MATERIAL_DATA& Data)
{
	std::string SetData = "";
	SetData += MAP_TYPE + std::string("MaterialName")+ SP_TYPE +Data.MaterialName + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Alpha") + SP_TYPE +std::to_string(Data.Alpha) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("DiffuseMap") + SP_TYPE +std::to_string(Data.DiffuseMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("NormalMap") + SP_TYPE +std::to_string(Data.NormalMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("EmissiveMap") + SP_TYPE +std::to_string(Data.EmissiveMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("ORMMap") + SP_TYPE +std::to_string(Data.ORMMap) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Emissive") + SP_TYPE +std::to_string(Data.Emissive) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Roughness")+ SP_TYPE +std::to_string(Data.Roughness) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Metallic") + SP_TYPE +std::to_string(Data.Metallic) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Tileing_X")+ SP_TYPE +std::to_string(Data.Tileing_X) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Tileing_Y")+ SP_TYPE +std::to_string(Data.Tileing_Y) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_R")+ SP_TYPE +std::to_string(Data.AddColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_G")+ SP_TYPE +std::to_string(Data.AddColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_B")+ SP_TYPE +std::to_string(Data.AddColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_A")+ SP_TYPE +std::to_string(Data.AddColor_A) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_R")+ SP_TYPE +std::to_string(Data.LimColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_G")+ SP_TYPE +std::to_string(Data.LimColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_B")+ SP_TYPE +std::to_string(Data.LimColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimFactor")+ SP_TYPE +std::to_string(Data.LimFactor) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimWidth")+ SP_TYPE +std::to_string(Data.LimWidth) + LINE_TYPE;
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

void EaterParser::SaveMaterial()
{
	//메테리얼 데이터를 저장
	std::string SetData = "";
	SetData += NODE_TYPE + std::string("EATERMAT") + LINE_TYPE;
	SetData += MAP_TYPE + std::string("MaterialName") + SP_TYPE + mMaterial_Data->Name + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Alpha") + SP_TYPE + std::to_string(mMaterial_Data->Alpha) + LINE_TYPE;

	if (mMaterial_Data->DiffuseMap == "")
	{
		SetData += MAP_TYPE + std::string("DiffuseMap") + SP_TYPE + std::string("0") + LINE_TYPE;
	}
	else{SetData += MAP_TYPE + std::string("DiffuseMap") + SP_TYPE + mMaterial_Data->DiffuseMap + LINE_TYPE;}

	if (mMaterial_Data->NormalMap == "")
	{
		SetData += MAP_TYPE + std::string("NormalMap") + SP_TYPE + std::string("0") + LINE_TYPE;
	}
	else { SetData += MAP_TYPE + std::string("NormalMap") + SP_TYPE + mMaterial_Data->NormalMap + LINE_TYPE; }

	if (mMaterial_Data->EmissiveMap == "")
	{
		SetData += MAP_TYPE + std::string("EmissiveMap") + SP_TYPE + std::string("0") + LINE_TYPE;
	}
	else { SetData += MAP_TYPE + std::string("EmissiveMap") + SP_TYPE + mMaterial_Data->EmissiveMap + LINE_TYPE; }

	if (mMaterial_Data->ORMMap == "")
	{
		SetData += MAP_TYPE + std::string("ORMMap") + SP_TYPE + std::string("0") + LINE_TYPE;
	}
	else { SetData += MAP_TYPE + std::string("ORMMap") + SP_TYPE + mMaterial_Data->ORMMap + LINE_TYPE; }

	SetData += MAP_TYPE + std::string("Emissive") + SP_TYPE + std::to_string(mMaterial_Data->Emissive) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Roughness") + SP_TYPE + std::to_string(mMaterial_Data->Roughness) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Metallic") + SP_TYPE + std::to_string(mMaterial_Data->Metallic) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Tileing_X") + SP_TYPE + std::to_string(mMaterial_Data->Tileing_X) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Tileing_Y") + SP_TYPE + std::to_string(mMaterial_Data->Tileing_Y) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("SkyLightIndex") + SP_TYPE + std::to_string(mMaterial_Data->SkyLightIndex) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_R") + SP_TYPE + std::to_string(mMaterial_Data->AddColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_G") + SP_TYPE + std::to_string(mMaterial_Data->AddColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("AddColor_B") + SP_TYPE + std::to_string(mMaterial_Data->AddColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_R") + SP_TYPE + std::to_string(mMaterial_Data->LimColor_R) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_G") + SP_TYPE + std::to_string(mMaterial_Data->LimColor_G) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimColor_B") + SP_TYPE + std::to_string(mMaterial_Data->LimColor_B) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimFactor") + SP_TYPE + std::to_string(mMaterial_Data->LimFactor) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("LimWidth") + SP_TYPE + std::to_string(mMaterial_Data->LimWidth) + LINE_TYPE;
	fputs(SetData.c_str(), WriteFile);
}

void EaterParser::SaveParticle()
{
	//파티클 데이터를 저장
	std::string SetData = "";
	SetData += NODE_TYPE + std::string("EATER_PARTICLE") + LINE_TYPE;
	SetData += MAP_TYPE + std::string("PlayTime") + SP_TYPE + std::to_string(mParticle_Data->PlayTime) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Loop") + SP_TYPE + std::to_string(mParticle_Data->Loop) + LINE_TYPE;

	SetData += MAP_TYPE + std::string("TextureName") + SP_TYPE + mParticle_Data->TextureName + LINE_TYPE;
	SetData += MAP_TYPE + std::string("RenderType") + SP_TYPE + std::to_string(mParticle_Data->Particle_Render_Type) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("ColorType") + SP_TYPE + std::to_string(mParticle_Data->Particle_LifeTime_Color_Type) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("ScaleType") + SP_TYPE + std::to_string(mParticle_Data->Particle_LifeTime_Scale_Type) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("MaxParticle") + SP_TYPE + std::to_string(mParticle_Data->MaxParticle) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("DelayTime") + SP_TYPE + std::to_string(mParticle_Data->DelayTime) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("RateOverTime") + SP_TYPE + std::to_string(mParticle_Data->RateOverTime) + LINE_TYPE;
	SetData += MAP_TYPE + std::string("Strength") + SP_TYPE + std::to_string(mParticle_Data->Strength) + LINE_TYPE;

	//생성 범위
	SetData += LIST_TYPE + std::string("Radius") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(3) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->ShapeRadius_X) + SP_TYPE + std::to_string(mParticle_Data->ShapeRadius_Y) + SP_TYPE + std::to_string(mParticle_Data->ShapeRadius_Z) + LINE_TYPE;

	//텍스쳐 타일링
	SetData += LIST_TYPE + std::string("Tiling") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->TextureTiling_X) + SP_TYPE + std::to_string(mParticle_Data->TextureTiling_Y)+ LINE_TYPE;
	//시작 방향
	SetData += LIST_TYPE + std::string("StartForce") + SP_TYPE + std::to_string(2) + SP_TYPE + std::to_string(3) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartForce_Min_X) + SP_TYPE + std::to_string(mParticle_Data->StartForce_Min_Y) + SP_TYPE + std::to_string(mParticle_Data->StartForce_Min_Z) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartForce_Max_X) + SP_TYPE + std::to_string(mParticle_Data->StartForce_Max_Y) + SP_TYPE + std::to_string(mParticle_Data->StartForce_Max_Z) + LINE_TYPE;
	//시작 색깔
	SetData += LIST_TYPE + std::string("StartColor") + SP_TYPE + std::to_string(2) + SP_TYPE + std::to_string(4) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Min_R) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Min_G) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Min_B) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Min_A) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Max_R) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Max_G) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Max_B) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->StartColor_Max_A) + LINE_TYPE;
	//생성 시간
	SetData += LIST_TYPE + std::string("StartLifeTime") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartLifeTime_Min) + SP_TYPE+ std::to_string(mParticle_Data->StartLifeTime_Max) + LINE_TYPE;
	//생성 크기
	SetData += LIST_TYPE + std::string("StartScale") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartScale_Min) + SP_TYPE + std::to_string(mParticle_Data->StartScale_Max) + LINE_TYPE;
	//생성 회전
	SetData += LIST_TYPE + std::string("StartRotation") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->StartRotation_Min) + SP_TYPE + std::to_string(mParticle_Data->StartRotation_Max) + LINE_TYPE;
	//생성 후 방향
	SetData += LIST_TYPE + std::string("LifeForce") + SP_TYPE + std::to_string(2) + SP_TYPE + std::to_string(3) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Min_X) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Min_Y) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Min_Z) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Max_X) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Max_Y) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeForce_Max_Z) + LINE_TYPE;
	//생성 후 색깔
	SetData += LIST_TYPE + std::string("LifeColor") + SP_TYPE + std::to_string(2) + SP_TYPE + std::to_string(4) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Min_R) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Min_G) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Min_B) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Min_A) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Max_R) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Max_G) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Max_B) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeColor_Max_A) + LINE_TYPE;
	//생성 후 크기
	SetData += LIST_TYPE + std::string("LifeScale") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeScale_Min) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeScale_Max) + LINE_TYPE;
	//생성 후 회전
	SetData += LIST_TYPE + std::string("LifeRotation") + SP_TYPE + std::to_string(1) + SP_TYPE + std::to_string(2) + LINE_TYPE;
	SetData += std::to_string(mParticle_Data->LifeRotation_Min) + SP_TYPE;
	SetData += std::to_string(mParticle_Data->LifeRotation_Max) + LINE_TYPE;
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

	int NodeCount = (int)mNodeList.size();
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




