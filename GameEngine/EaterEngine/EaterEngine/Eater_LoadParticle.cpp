#include "Eater_LoadParticle.h"
#include "EaterHeader.h"
#include "EngineData.h"
#include "LoadManager.h"
#include <string>
Eater_LoadParticle::Eater_LoadParticle()
{
}

Eater_LoadParticle::~Eater_LoadParticle()
{
}

void Eater_LoadParticle::LoadData(std::string& Path)
{
	LoadParticleData* mParticle = new LoadParticleData();

	EATER_OPEN_READ_FILE(Path);
	int NodeCount = EATER_GET_NODE_COUNT();
	for (int i = 0; i < NodeCount; i++)
	{
		mParticle->TextrueName  = EATER_GET_MAP(i, "TextureName");
		mParticle->RenderType   = std::stoi(EATER_GET_MAP(i, "RenderType"));
		mParticle->ColorType	= std::stoi(EATER_GET_MAP(i, "ColorType"));
		mParticle->ScaleType	= std::stoi(EATER_GET_MAP(i, "ScaleType"));
		mParticle->MaxParticle	= std::stoi(EATER_GET_MAP(i, "MaxParticle"));
		mParticle->RateOverTime = std::stof(EATER_GET_MAP(i, "RateOverTime"));
		mParticle->Strength		= std::stof(EATER_GET_MAP(i, "Strength"));

		mParticle->PlayTime		= std::stof(EATER_GET_MAP(i, "PlayTime"));
		mParticle->Loop			= std::stoi(EATER_GET_MAP(i, "Loop"));

		std::vector<float> Data;
		EATER_GET_LIST_CHOICE(i, "Radius");
		EATER_GET_LIST(&Data, 0);
		mParticle->Radius.x = Data[0];
		mParticle->Radius.y = Data[1];
		mParticle->Radius.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "Tiling");
		EATER_GET_LIST(&Data, 0);
		mParticle->Tiling.x = Data[0];
		mParticle->Tiling.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartForce");
		EATER_GET_LIST(&Data, 0);
		mParticle->StartForce_Min.x = Data[0];
		mParticle->StartForce_Min.y = Data[1];
		mParticle->StartForce_Min.z = Data[2];
		EATER_GET_LIST(&Data, 1);
		mParticle->StartForce_Max.x = Data[0];
		mParticle->StartForce_Max.y = Data[1];
		mParticle->StartForce_Max.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "StartColor");
		EATER_GET_LIST(&Data, 0);
		mParticle->StartColor_Min.x = Data[0];
		mParticle->StartColor_Min.y = Data[1];
		mParticle->StartColor_Min.z = Data[2];
		mParticle->StartColor_Min.w = Data[3];
		EATER_GET_LIST(&Data, 1);
		mParticle->StartColor_Max.x = Data[0];
		mParticle->StartColor_Max.y = Data[1];
		mParticle->StartColor_Max.z = Data[2];
		mParticle->StartColor_Max.w = Data[3];

		EATER_GET_LIST_CHOICE(i, "StartLifeTime");
		EATER_GET_LIST(&Data, 0);
		mParticle->StartLifeTime.x = Data[0];
		mParticle->StartLifeTime.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartScale");
		EATER_GET_LIST(&Data, 0);
		mParticle->StartScale.x = Data[0];
		mParticle->StartScale.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "StartRotation");
		EATER_GET_LIST(&Data, 0);
		mParticle->StartRotation.x = Data[0];
		mParticle->StartRotation.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "LifeForce");
		EATER_GET_LIST(&Data, 0);
		mParticle->LifeForce_Min.x = Data[0];
		mParticle->LifeForce_Min.y = Data[1];
		mParticle->LifeForce_Min.z = Data[2];
		EATER_GET_LIST(&Data, 1);
		mParticle->LifeForce_Max.x = Data[0];
		mParticle->LifeForce_Max.y = Data[1];
		mParticle->LifeForce_Max.z = Data[2];

		EATER_GET_LIST_CHOICE(i, "LifeColor");
		EATER_GET_LIST(&Data, 0);
		mParticle->LifeColor_Min.x = Data[0];
		mParticle->LifeColor_Min.y = Data[1];
		mParticle->LifeColor_Min.z = Data[2];
		mParticle->LifeColor_Min.w = Data[3];
		EATER_GET_LIST(&Data, 1);
		mParticle->LifeColor_Max.x = Data[0];
		mParticle->LifeColor_Max.y = Data[1];
		mParticle->LifeColor_Max.z = Data[2];
		mParticle->LifeColor_Max.w = Data[3];

		EATER_GET_LIST_CHOICE(i, "LifeScale");
		EATER_GET_LIST(&Data, 0);
		mParticle->LifeScale.x = Data[0];
		mParticle->LifeScale.y = Data[1];

		EATER_GET_LIST_CHOICE(i, "LifeRotation");
		EATER_GET_LIST(&Data, 0);
		mParticle->LifeRotation.x = Data[0];
		mParticle->LifeRotation.y = Data[1];
	}
	EATER_CLOSE_READ_FILE();
	std::size_t Start	= Path.rfind('/') + 1;
	std::size_t End		= Path.rfind('.') - Start;
	std::string FileName = Path.substr(Start, End);

	LoadManager::ParticleList.insert({ FileName, mParticle });
}
